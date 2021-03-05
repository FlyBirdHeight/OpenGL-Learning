//
//  perlin.cpp
//  opengl
//
//  Created by adsionli on 2021/3/4.
//  Copyright © 2021 adsionli. All rights reserved.
//

#include "perlin.hpp"
#include <fstream>
void PerlinNoise::init(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Perlin Noise Template", NULL, NULL);
    glfwMakeContextCurrent(window);
    
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }
    Shader shader("./shader/vs/noise/perlin.vs","./shader/fs/noise/perlin.fs");
    shader.use();
    
    while (!glfwWindowShouldClose(window)) {
        //float currentTime = glfwGetTime();
        
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glfwSwapBuffers(window);
    }
    
    glfwDestroyWindow(window);
 
    glfwTerminate();
}
//创建晶格并生成梯度向量
void PerlinNoise::generateMesh(){
    unsigned seed = 2016;
    std::mt19937 generate(seed);
    std::uniform_real_distribution<float> distribution{.0, 1.};
    auto randomData = std::bind(distribution, generate);
    float gradientLen2 = 0.0;
    for(unsigned i = 0; i < this->g_tableSize; ++i){
        this->gradient[i] = glm::fvec2(2 * randomData() - 1, 2 * randomData() - 1);
        gradientLen2 = this->gradient[i].length();
        this->gradient[i] /= sqrtf(gradientLen2);
        this->permutationTable[i] = i;
    }
    //perlin noise中的p数组，负责在提取晶格坐标时
    int p[512] = { 151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
        190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
        88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
        77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
        102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
        135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
        5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
        223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
        129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
        251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
        49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
        138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
    };
    for(unsigned i = 0; i < this->g_tableSize; i++){
        this->permutationTable[256+i] = this->permutationTable[i] = p[i];
    }
}
//生成对应的晶格数据
float PerlinNoise::generateMeshData(glm::fvec2 point){
    //左下角晶格顶点
    glm::fvec2 mesh_left_bottom_point = glm::floor(point);
    //最下角晶格顶点的x,y坐标
    int mesh_point_x = ((int) std::floor(point.x)) % this->g_tableSize;
    int mesh_point_y = ((int) std::floor(point.y)) % this->g_tableSize;
    //晶格点其余顶点
    glm::fvec2 mesh_right_bottom_point(mesh_point_x + 1.0, mesh_point_y);
    glm::fvec2 mesh_left_top_point(mesh_point_x, mesh_point_y + 1.0);
    glm::fvec2 mesh_right_top_point(mesh_point_x + 1.0, mesh_point_y + 1.0);
    
    //左下角晶格顶点指向晶格内的向量
    glm::fvec2 mesh_left_bottom_vector = point - mesh_left_bottom_point;
    //2维晶格顶点剩下三个点指向晶格内点的向量
    glm::fvec2 mesh_right_bottom_vector = mesh_left_bottom_vector + glm::vec2(1.0, 0.0);
    glm::fvec2 mesh_left_top_vector = mesh_left_bottom_vector + glm::vec2(0.0, 1.0);
    glm::fvec2 mesh_right_top_vector = mesh_left_bottom_vector + glm::vec2(1., 1.);
    
    //获取四个晶格顶点的梯度向量;
    glm::fvec2 mesh_left_bottom_gradient = this->getIndex(mesh_point_x, mesh_point_y);
    glm::fvec2 mesh_left_top_gradient = this->getIndex((int)mesh_left_top_point.x, (int)mesh_left_top_point.y);
    glm::fvec2 mesh_right_bottom_gradient = this->getIndex((int)mesh_right_bottom_point.x, (int)mesh_right_bottom_point.y);
    glm::fvec2 mesh_right_top_gradient = this->getIndex((int)mesh_right_top_point.x, (int)mesh_right_top_point.y);
    
    //获取缓和曲线处理过的点
    float u = this->smoothCurve(point.x - mesh_point_x);
    float v = this->smoothCurve(point.y - mesh_point_y);
    //线性混合
    float mesh_bottom = glm::mix(glm::dot(mesh_left_bottom_vector, mesh_left_bottom_gradient), glm::dot(mesh_right_bottom_vector, mesh_right_bottom_gradient), u);
    float mesh_top = glm::mix(glm::dot(mesh_left_top_vector, mesh_left_top_gradient), glm::dot(mesh_right_top_vector, mesh_right_top_gradient), u);
    float returnData = glm::mix(mesh_bottom, mesh_top, v);
    
    return returnData;
}

//梯度向量获取
glm::fvec2 PerlinNoise::getIndex(int x, int y){
    return this->gradient[(x + this->permutationTable[y]) % 256];
}
//缓和曲线
float PerlinNoise::smoothCurve(float data){
    float returnData = data * data * (3.0 - 2.0 * data);
    
    return returnData;
}

//测试
void PerlinNoise::testHash(int x, int y){
    this->generateMesh();
    std::ofstream newFile("./resources/noise/test.ppm");
//    newFile << "数据测试"  << std::endl;
    int xx=400, yy=400;
    newFile << "P3" << std::endl << xx << " " << yy << " " << std::endl << "255" << std::endl;
    for(float i = 0.0; i < 50.0; i += 0.05){
        for(float j = 0.0; j < 50.0; j += 0.05){
            glm::vec2 test(i, j);
            float sum = this->generateMeshData(test);
            int b = (sum + 1)*255.0 / 2.0;
//            std::cout << sum << std::endl;
            newFile << b << " " << b << " " << b << std::endl;;
        }
    }
    newFile.close();
    std::cout << "Write Success" << std::endl;
}

