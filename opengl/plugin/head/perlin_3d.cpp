//
//  perlin_3d.cpp
//  opengl
//
//  Created by adsionli on 2021/3/5.
//  Copyright © 2021 adsionli. All rights reserved.
//

#include "perlin_3d.hpp"
#include <fstream>
void PerlinNoise3D::init(){
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
void PerlinNoise3D::generateMesh(){
    unsigned seed = 2016;
    std::mt19937 generate(seed);
    std::uniform_real_distribution<float> distribution{.0, 1.};
    auto randomData = std::bind(distribution, generate);
    float gradientLen2 = 0.0;
    for(unsigned i = 0; i < this->g_tableSize; ++i){
        this->gradient[i] = glm::fvec3(2 * randomData() - 1, 2 * randomData() - 1, 2 * randomData() - 1);
        gradientLen2 = this->gradient[i].length();
        this->gradient[i] /= sqrtf(gradientLen2);
        this->permutationTable[i] = i;
    }
    std::uniform_int_distribution<int> distributionInt{0, 255};
    auto diceInt = std::bind(distributionInt, generate);
    for(unsigned int i = 0; i < this->g_tableSize; ++i){
        std::swap(this->permutationTable[i], this->permutationTable[diceInt() % this->g_tableSize]);
    }
    for(unsigned int i = 0; i < this->g_tableSize; ++i){
        this->permutationTable[this->g_tableSize + i] = this->permutationTable[i];
    }
}
//生成对应的晶格数据
float PerlinNoise3D::generateMeshData(glm::fvec3 point){
    //最下角晶格顶点的x,y,z坐标
    int x = ((int) std::floor(point.x));
    int y = ((int) std::floor(point.y));
    int z = ((int) std::floor(point.z));
    
    int x1 = (x + 1);
    int y1 = (y + 1);
    int z1 = (z + 1);
    //晶格点顶点
    glm::fvec3 xyz1(x, y, z);
    glm::fvec3 xyz2(x1, y, z);
    glm::fvec3 xyz3(x, y1, z);
    glm::fvec3 xyz4(x1, y1, z);
    
    glm::fvec3 xyz5(x, y, z1);
    glm::fvec3 xyz6(x1, y, z1);
    glm::fvec3 xyz7(x, y1, z1);
    glm::fvec3 xyz8(x1, y1, z1);
    
    //左下角晶格顶点指向晶格内的向量
    float tx = point.x - ((int)std::floor(point.x));
    float ty = point.y - ((int)std::floor(point.y));
    float tz = point.z - ((int)std::floor(point.z));
    float xv0 = tx;float yv0 = ty;float zv0 = tz;
    float xv1 = tx - 1;float yv1 = ty - 1;float zv1 = tz - 1;
    //3维晶格顶点指向晶格内点的向量
    glm::fvec3 xyzv1(xv0, yv0, zv0);
    glm::fvec3 xyzv2(xv1, yv0, zv0);
    glm::fvec3 xyzv3(xv0, yv1, zv0);
    glm::fvec3 xyzv4(xv1, yv1, zv0);
    glm::fvec3 xyzv5(xv0, yv0, zv1);
    glm::fvec3 xyzv6(xv1, yv0, zv1);
    glm::fvec3 xyzv7(xv0, yv1, zv1);
    glm::fvec3 xyzv8(xv1, yv1, zv1);
    
    //获取四个晶格顶点的梯度向量;
    glm::fvec3 xyzg1 = this->getIndex(x, y, z);
    glm::fvec3 xyzg2 = this->getIndex(x1, y, z);
    glm::fvec3 xyzg3 = this->getIndex(x, y1, z);
    glm::fvec3 xyzg4 = this->getIndex(x1, y1, z);
    glm::fvec3 xyzg5 = this->getIndex(x, y, z1);
    glm::fvec3 xyzg6 = this->getIndex(x1, y, z1);
    glm::fvec3 xyzg7 = this->getIndex(x, y1, z1);
    glm::fvec3 xyzg8 = this->getIndex(x1, y1, z1);
    
    //获取缓和曲线处理过的点
    float u = this->smoothCurve(point.x - x);
    float v = this->smoothCurve(point.y - y);
    float w = this->smoothCurve(point.z - z);
    
    //线性混合
    float lerp1 = this->lerp(glm::dot(xyzv1, xyzg1), glm::dot(xyzv2, xyzg2), u);
    float lerp2 = this->lerp(glm::dot(xyzv3, xyzg3), glm::dot(xyzv4, xyzg4), u);
    float a = this->lerp(lerp1, lerp2, v);
    
    float lerp3 = this->lerp(glm::dot(xyzv5, xyzg5), glm::dot(xyzv6, xyzg6), u);
    float lerp4 = this->lerp(glm::dot(xyzv7, xyzg7), glm::dot(xyzv8, xyzg8), u);
    float b = this->lerp(lerp3, lerp4, v);
    
    return this->lerp(a, b, w);
}

//梯度向量获取   g(i, j, k) = P[ ( i + P[ (j + P[k]) mod 256 ] ) mod 256 ]
//  a = p[y + p[z]]; b = (a % 256 + 256) % 256; c = a + b; d = (c % 256 + 256) % 256
glm::fvec3 PerlinNoise3D::getIndex(int x, int y, int z){
    int a = this->permutationTable[y + this->permutationTable[z]];
    int b = (a % 256 + 256) % 256;
    int c = a + b;
    int d = (c % 256 + 256) % 256;
    return this->gradient[d];
}
//缓和曲线
float PerlinNoise3D::smoothCurve(float t){
    float returnData = t * t * t * (t * (t * 6. - 15.) + 10.);
    
    return returnData;
}
//线性混合
float PerlinNoise3D::lerp(float a, float b, float v){
    return a * (1 - v) + b * v;
}
//测试
void PerlinNoise3D::testHash(){
    this->generateMesh();
    std::ofstream newFile("./resources/noise/perlin_noise2.ppm");
    int xx=400, yy=400;
    newFile << "P3" << std::endl << xx << " " << yy << " " << std::endl << "255" << std::endl;
    for(float i = 0.0; i < 25.0; i += 0.05){
        for(float j = 0.0; j < 25.0; j += 0.05){
            glm::fvec3 p(i, j, 7.89101112131415);
            float sum = 0, frequency = 1, amplitude = 1;
            for(int k = 0; k < 4; ++k, frequency *= 2.0, amplitude *= 0.5){
                sum += abs(this->generateMeshData(glm::fvec3(p.x * frequency, p.y * frequency, p.z * frequency))) * amplitude;
            }
            int b = (sum + 1)*255.0 / 2.0;
            newFile << b << " " << b << " " << b << std::endl;
        }
    }
    newFile.close();
    std::cout << "Write Success" << std::endl;
}


