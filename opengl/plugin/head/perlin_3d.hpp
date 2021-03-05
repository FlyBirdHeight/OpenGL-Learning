//
//  perlin_3d.hpp
//  opengl
//  柏林噪声3维
//  Created by adsionli on 2021/3/5.
//  Copyright © 2021 adsionli. All rights reserved.
//

#ifndef perlin_3d_hpp
#define perlin_3d_hpp
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <random>
#include <cmath>
#include "shaders.h"
class PerlinNoise3D{
    //定义纹理大小
    static const unsigned int g_tableSize = 256;
    //定义随机排列数组大小
    static const unsigned int p_tableSize = g_tableSize - 1;
public:
    //生成晶格数据
    void generateMesh();
    //生成对应的晶格数据
    float generateMeshData(glm::fvec3 point);
    //缓和曲线
    float smoothCurve(float data);
    //梯度向量下标获取
    glm::fvec3 getIndex(int x, int y, int z);
    //线性混合
    float lerp(float a, float b, float v);
    //初始化界面
    void init();
    //测试生成晶格
    void testHash();

private:
    unsigned int SCR_WIDTH = 800, SCR_HEIGHT = 600;
    //定义查找表
    glm::fvec3 gradient[g_tableSize];
    //定义随机排列数组
    unsigned permutationTable[g_tableSize * 2];
};

#endif /* perlin_3d_hpp */
