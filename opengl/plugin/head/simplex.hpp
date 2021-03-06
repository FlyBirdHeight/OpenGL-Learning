//
//  simplex.hpp
//  opengl
//
//  Created by adsionli on 2021/3/8.
//  Copyright © 2021 adsionli. All rights reserved.
//

#ifndef simplex_hpp
#define simplex_hpp
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <random>
#include <cmath>
#include "shaders.h"
#define K2 0.166666667
#define K1 0.333333333
class SimplexNoise{
    static const unsigned int g_Size = 256;
    static const unsigned int grad_Size = 12;
public:
    SimplexNoise();
    //线性插值
    double lerp(double a, double b, double c);
    //噪声贡献值计算
    double contribution(glm::dvec3 a, glm::dvec3 b, glm::dvec3 c, glm::dvec3 d, glm::dvec3 g1, glm::dvec3 g2, glm::dvec3 g3, glm::dvec3 g4);
    //从单形转换到超方体晶格中
    glm::dvec3 transformMesh(glm::dvec3 p);
    //从超方体晶格转换到单形中
    glm::dvec3 reverseMesh(glm::dvec3 p);
    //生成查找梯度向量的全排列数组
    void generatePerumition();
    //获取对应单形顶点的梯度向量
    glm::dvec3  getGradient(int hash, glm::dvec3 p);
    //生成数据
    double handleData(glm::dvec3 point);
    //排列大小
    void order(glm::dvec3 point);
    //分型布朗
    double fbm1(double octaves, double x, double y, double z);
    double fbm2(double octaves, double x, double y, double z);
    //测试
    void test();
private:
    unsigned int permutation[g_Size] = { 151,160,137,91,90,15,
        131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
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
    
    //定义随机排列数组
    unsigned int permutationTable[g_Size * 2];
    //随机梯度向量数组
    glm::dvec3 gradientData[grad_Size] = {
        glm::dvec3(1,1,0),
        glm::dvec3(-1,1,0),
        glm::dvec3(1,-1,0),
        glm::dvec3(-1,-1,0),
        glm::dvec3(1,0,1),
        glm::dvec3(-1,0,1),
        glm::dvec3(1,0,-1),
        glm::dvec3(-1,0,-1),
        glm::dvec3(0,1,1),
        glm::dvec3(0,-1,1),
        glm::dvec3(0,1,-1),
        glm::dvec3(0,-1,-1)
    };
    //大小值调整
    glm::dvec3 order_data[3] = {
        glm::dvec3(1.0, 0.0, 0.0),
        glm::dvec3(0.0, 1.0, 0.0),
        glm::dvec3(0.0, 0.0, 1.0)
    };
};

#endif /* simplex_hpp */
