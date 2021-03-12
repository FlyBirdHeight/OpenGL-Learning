//#define STB_IMAGE_IMPLEMENTATION    // include之前必须定义
//#include "stb_image.h"
#include "terrain.hpp"
Terrain::Terrain(){
    this->camera = new Camera(glm::vec3(this->originX, 20.0, this->originY));
}

//初始化
void Terrain::init(){
    std::ofstream newFile("./resources/noise/terrain.txt");
    //三角形顶点索引数组
    std::vector<int> indices = this->generate_indice();
    std::vector<float> verticesData;
    for(unsigned int y = 0; y < this->yMapChunk; ++y){
        for(unsigned int x = 0; x < this->xMapChunk; ++x){
            std::vector<float> noiseMap;
            noiseMap = this->generate_noise(x, y);
            std::vector<float> vertices = this->generate_vertices(noiseMap, x, y);
            verticesData.insert(verticesData.end(), vertices.begin(), vertices.end());
        }
    }
    for(int i = 0; i < verticesData.size(); ++i){
        if(i % 3 == 0){
            newFile << "\n";
        }
        newFile << verticesData[i] << "      ";
    }
    newFile << "\n";
    std::cout << "Write Success" << std::endl;
}

//生成构成平面的三角形顶点，主要用于顶点索引对象使用
std::vector<int> Terrain::generate_indice(){
    std::vector<int> returnData;
    for(unsigned int i = 0; i < chunkHeight; i++){
        for(unsigned int j  = 0; j < chunkWidth; j++){
            int pos = j + i * chunkWidth;
            if(i == chunkWidth - 1 || j == chunkHeight - 1){
                continue;
            }else{
                returnData.push_back(pos + chunkWidth);
                returnData.push_back(pos);
                returnData.push_back(pos + 1 + chunkWidth);
                returnData.push_back(pos + 1);
                returnData.push_back(pos + 1+ chunkWidth);
                returnData.push_back(pos);
            }
        }
    }
    
    return returnData;
}
//对每一个网格生成相对应的噪声值，10 * 10的大网格内部的127 * 127的小网格
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
//生成网格数据
std::vector<float> Terrain::generate_vertices(const std::vector<float> &noise_map, int xOffset, int yOffset){
    std::vector<float> vertices;
    for(unsigned int i = 0; i < this->chunkHeight; ++i){
        for(unsigned int j = 0; j < this->chunkWidth; ++j){
            vertices.push_back(j + xOffset * this->chunkWidth);
            float scaleNoise = std::fmax(std::pow(noise_map[j + i * this->chunkHeight] * 1.1, 3) * this->meshHeight, this->horizontal * 0.5 * this->meshHeight);
            
            vertices.push_back(scaleNoise);
            vertices.push_back(i + yOffset * this->chunkHeight);
        }
    }
    
    return vertices;
}
//生成地形群落
std::vector<float> Terrain::generate_biome(const std::vector<float> &vertices, std::vector<int> &plant, int xOffset, int yOffset){
    std::vector<float> color;
    
    return color;
}
