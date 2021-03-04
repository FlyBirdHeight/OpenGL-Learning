//
//  perlin.hpp
//  opengl
//
//  Created by adsionli on 2021/3/4.
//  Copyright © 2021 adsionli. All rights reserved.
//

#ifndef perlin_hpp
#define perlin_hpp
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <random>
#include <cmath>
#include "shaders.h"

class PerlinNoise{
    //定义纹理大小
    static const unsigned int g_tableSize = 256;
    //定义随机排列数组大小
    static const unsigned int p_tableSize = g_tableSize - 1;
public:
    //生成晶格
    void generateMesh();
    //初始化界面
    void init();
    //确保噪声函数的任何输入值都可以映射到该随机值数组中的有且只有一个值
    int hash(const int &x, const int &y) const;
    
    void testHash();

private:
    unsigned int SCR_WIDTH = 800, SCR_HEIGHT = 600;
    //定义查找表
    glm::vec2 gradient[g_tableSize];
    //定义随机排列数组
    unsigned permutationTable[g_tableSize * 2];
};

#endif /* perlin_hpp */
