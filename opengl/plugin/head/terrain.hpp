#include <string>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <cmath>
#include <random>
#include "shaders.h"
#include "perlin.hpp"
#include "camera.h"
class Terrain{
public:
    Terrain();
    //地形群落
    struct terrainColor {
        terrainColor(float _height, glm::vec3 _color) {
            height = _height;
            color = _color;
        };
        float height;
        glm::vec3 color;
    };

    //植物
    struct plant {
        //植物类型（花，树）
        std::string type;
        //植物坐标
        float xpos;
        float ypos;
        float zpos;
        //植物偏移量
        int xOffset;
        int yOffset;
        
        plant(std::string _type, float _xpos, float _ypos, float _zpos, int _xOffset, int _yOffset) {
            type = _type;
            xpos = _xpos;
            ypos = _ypos;
            zpos = _zpos;
            xOffset = _xOffset;
            yOffset = _yOffset;
        }
    };
    //总区域划分数, 水平方向，虽然写的是y实际是z轴
    int xMapChunk = 10;
    int yMapChunk = 10;
    //大区域中对小区域划分
    int chunkWidth = 127;
    int chunkHeight = 127;
    //相机外网格数量
    int gridPosX = 0;
    int gridPosY = 0;
    //视点位置
    float originX = (chunkWidth  * xMapChunk) / 2 - chunkWidth / 2;
    float originY = (chunkHeight * yMapChunk) / 2 - chunkHeight / 2;
    //水平面高度
    float horizontal = 0.1;
    //需要渲染的最大网格距离
    int chunk_render_distance = 3;
    //噪声相关参数设置
    int octaves = 5;//度数
    float meshHeight = 32;//顶点缩放
    float noiseScale = 64;//水平缩放
    //fbm中的一些参数设置
    float persistence = 0.5;
    float lacunarity = 2;
    //植物模型的缩放以及亮度值
    float MODEL_SCALE = 3;
    float MODEL_BRIGHTNESS = 6;
    //初始化数据
    void init(GLFWwindow* window);
    //创建窗口
    void createWindow();
    //渲染
    void render(GLFWwindow* window, std::vector<unsigned int> mapChunkVao,std::vector<unsigned int> treeVao, std::vector<unsigned int> flowerVao);
private:
    PerlinNoise perlinNoise;
    //生成噪声高度
    std::vector<float> generate_noise(int xOffset, int yOffset);
    //生成构成平面的三角形顶点
    std::vector<int> generate_indice();
    //生成每一个网格区域的点坐标
    std::vector<float> generate_vertices(const std::vector<float> &noise_map, int xOffset, int yOffset);
    //生成法线向量
    std::vector<float> generate_normal(const std::vector<int> &indices, const std::vector<float> &vertices);
    //生成群落颜色
    std::vector<float> generate_biome(const std::vector<float> &vertices, std::vector<plant> &plants, int xOffset, int yOffset);
    //处理颜色取值(glsl中需要处理为0.0 - 1.0范围内)
    glm::vec3 handleColor(int r, int g, int b);
    //生成地图区域
    void generate_map_chunk(unsigned int &mapChunkVao, std::vector<float> vertices, std::vector<int> indices, std::vector<float> colorCard, std::vector<float> normals);
    //加载模型
    void load_model(unsigned int &VAO, std::string filename, std::string materialName);
    //设置模型
    void set_model(std::vector<unsigned int> &plant_chunk, std::string type, std::vector<plant> &plants, std::string filename, std::string materialName);
};
