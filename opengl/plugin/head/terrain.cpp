//#define STB_IMAGE_IMPLEMENTATION    // include之前必须定义
//#include "stb_image.h"
#include "terrain.hpp"
/**
 说明一下，为什么这里都是用float存数据的问题
 因为我们在使用opengl去绑定着色器的时候，我们需要确定数据的步长，如果使用glm::vec3的话，就不能很好地让opengl去理解，也没办使用顶点缓冲对象、顶点数组对象和顶点索引对象了
 所以，我们这里全部都要使用vector<float>的形式，去存储顶点坐标、颜色、法线、三角形顶点索引数据
 */
Terrain::Terrain(){
    this->camera = new Camera(glm::vec3(this->originX, 20.0, this->originY));
}

//初始化
void Terrain::initData(){
    std::ofstream newFile("./resources/noise/terrain.txt");
    //三角形顶点索引数组
    std::vector<int> indices = this->generate_indice();
    //地形顶点数据集合
    std::vector<float> verticesData;
    //地形着色数据
    std::vector<float> colorCard;
    //地形附着植物数据
    std::vector<plant> plants;
    for(unsigned int y = 0; y < this->yMapChunk; ++y){
        for(unsigned int x = 0; x < this->xMapChunk; ++x){
            //生成噪声值
            std::vector<float> noiseMap;
            noiseMap = this->generate_noise(x, y);
            //生成每个网格中的坐标点
            std::vector<float> vertices = this->generate_vertices(noiseMap, x, y);
            verticesData.insert(verticesData.end(), vertices.begin(), vertices.end());
            //生成地形颜色群落
            std::vector<float> returnColor = this->generate_biome(vertices, plants, x, y);
            colorCard.insert(colorCard.end(), returnColor.begin(), returnColor.end());
        }
    }
    //生成法线数据
    std::vector<float> normals = this->generate_normal(indices, verticesData);

}

//生成构成平面的三角形顶点，主要用于顶点索引对象使用
std::vector<int> Terrain::generate_indice(){
    std::vector<int> returnData;
    for(unsigned int i = 0; i < this->chunkHeight; i++){
        for(unsigned int j  = 0; j < this->chunkWidth; j++){
            int pos = j + i * this->chunkWidth;
            if(i == this->chunkWidth - 1 || j == this->chunkHeight - 1){
                continue;
            }else{
                returnData.push_back(pos + this->chunkWidth);
                returnData.push_back(pos);
                returnData.push_back(pos + 1 + this->chunkWidth);
                returnData.push_back(pos + 1);
                returnData.push_back(pos + 1+ this->chunkWidth);
                returnData.push_back(pos);
            }
        }
    }
    
    return returnData;
}
/**
 对每一个网格生成相对应的噪声值，10 * 10的大网格内部的127 * 127的小网格
 @param {int} xOffset 横向大区域偏移量
 @param {int} yOffset z方向大区域偏移量
 */
std::vector<float> Terrain::generate_noise(int xOffset, int yOffset){
    std::vector<float> noiseValues;
    
    for(unsigned int i = 0; i < this->chunkHeight; ++i){
        for(unsigned int j = 0; j < this->chunkWidth; j++){
            float noiseData = this->perlinNoise.getNoiseByFbm(this->octaves, (j + xOffset * chunkWidth) / this->noiseScale, (i + yOffset * chunkHeight) / this->noiseScale);
            noiseValues.push_back(noiseData);
        }
    }
    
    return noiseValues;
}
/**
 生成网格数据
 @param {const std::vector<float>} noise_map 噪声数据值
 @param {int} xOffset 横向大区域偏移量
 @param {int} yOffset z方向大区域偏移量
 */
std::vector<float> Terrain::generate_vertices(const std::vector<float> &noise_map, int xOffset, int yOffset){
    std::vector<float> vertices;
    unsigned seed = 1000000;
    std::mt19937 generate(seed);
    std::uniform_real_distribution<float> terrainHeight{32.0, 35.0};
    auto dice = std::bind(terrainHeight, generate);
    for(unsigned int i = 0; i < this->chunkHeight; ++i){
        for(unsigned int j = 0; j < this->chunkWidth; ++j){
            vertices.push_back(j + xOffset * this->chunkWidth);
            float scaleNoise = std::fmax(std::pow(noise_map[j + i * this->chunkHeight] * 1.1, 3) * this->meshHeight, this->horizontal * 0.5 * this->meshHeight);
            if(isinf(scaleNoise)){
                scaleNoise = dice();
            }
            vertices.push_back(scaleNoise);
            vertices.push_back(i + yOffset * this->chunkHeight);
        }
    }
    
    return vertices;
}
/**
 生成地形群落
 @param {std::vector<float>} vertices 网格顶点坐标集合
 @param {std::vector<int>} plant 植物集合
 @param {int} xOffset 横向大区域偏移量
 @param {int} yOffset z方向大区域偏移量
 @return {std::vector<float>} 颜色数据组
 */
std::vector<float> Terrain::generate_biome(const std::vector<float> &vertices, std::vector<plant> &plants, int xOffset, int yOffset){
    std::vector<float> colors;
    std::vector<terrainColor> biomeColors;
    glm::vec3 color = this->handleColor(255, 255, 255);
    //设置颜色，根据不同的高度设置
    biomeColors.push_back(terrainColor(this->horizontal * 0.5, this->handleColor(60,  95, 190)));   // 深水
    biomeColors.push_back(terrainColor(this->horizontal, this->handleColor(60, 100, 190)));  // 浅水
    biomeColors.push_back(terrainColor(0.15, this->handleColor(210, 215, 130)));                // 陆地
    biomeColors.push_back(terrainColor(0.30, this->handleColor( 95, 165,  30)));                // 草地1
    biomeColors.push_back(terrainColor(0.40, this->handleColor( 65, 115,  20)));                // 草地2
    biomeColors.push_back(terrainColor(0.50, this->handleColor( 90,  65,  60)));                // Rock 1
    biomeColors.push_back(terrainColor(0.80, this->handleColor( 75,  60,  55)));                // Rock 2
    biomeColors.push_back(terrainColor(1.00, this->handleColor(255, 255, 255)));                // 积雪
    std::string plantType;
    //老样子，遍历一下顶点坐标，然后来分配不同高度的着色, 注意这里是要高度，所以是要用Y的坐标，所以从1的位置开始，每次跳跃3个高度
    for(int i = 1; i < vertices.size(); i += 3){
        for(int j = 0; j < biomeColors.size(); j++){
            if(vertices[i] <= biomeColors[j].height * this->meshHeight){
                color = biomeColors[j].color;
                //这里是用来判断是否需要放置植被的，可以进行自定义，随意都可以，但是需要在陆地以上
                if(j == 3){
                    if(rand() % 1000 < 5){
                        if(rand() % 100 < 70){
                            plantType = "flower";
                        }else{
                            plantType = "tree";
                        }
                        plants.push_back(plant(plantType, vertices[i-1], vertices[i], vertices[i + 1], xOffset, yOffset));
                    }
                }
                break;
            }
        }
        colors.push_back(color.x);
        colors.push_back(color.y);
        colors.push_back(color.z);
    }
    
    
    return colors;
}
/**
 处理颜色数据
 */
glm::vec3 Terrain::handleColor(int r, int g, int b){
    return glm::vec3(r / 255.0, g / 255.0, b / 255.0);
}
/**
 生成法向量
 @param {std::vector<int>} indices 三角形平面索引数据
 @param {std::vector<float>} vertices 网格顶点数据
 */
std::vector<float> Terrain::generate_normal(const std::vector<int> &indices, const std::vector<float> &vertices){
    std::vector<float> normals;
    glm::vec3 normal;
    //这里计算每个三角形平面的法向量，所以要根据顶点索引数组进行坐标点的取值，然后进行向量叉乘，获得法向量，同时由于这里使用的是右手螺旋，所以最后要记得取反
    for(int i= 0; i < indices.size(); i += 3){
        std::vector<glm::vec3> vert;
        for(int j = 0; j < 3; j++){
            //因为存储坐标的3个为一个步长，所以这里乘以3，而且我们需要三角形三个顶点的坐标，所以这里要循环三次
            int pos = indices[i + j] * 3;
            vert.push_back(glm::vec3(vertices[pos], vertices[pos + 1], vertices[pos + 2]));
        }
        glm::vec3 vectorA = vert[1] - vert[0];
        glm::vec3 vectorB = vert[2] - vert[0];
        glm::vec3 normal = glm::normalize(-glm::cross(vectorA, vectorB));
        normals.push_back(normal.x);
        normals.push_back(normal.y);
        normals.push_back(normal.z);
    }
    
    return normals;
}
