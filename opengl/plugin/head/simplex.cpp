//
//  simplex.cpp
//  opengl
//
//  Created by adsionli on 2021/3/8.
//  Copyright © 2021 adsionli. All rights reserved.
//

#include "simplex.hpp"
#include <fstream>

SimplexNoise::SimplexNoise(){
    this->generatePerumition();
}
//生成数据
double SimplexNoise::handleData(glm::dvec3 point){
    //首先获得在超方体下的坐标点
    glm::dvec3 iPoint = this->transformMesh(point);
    int x = ((int) std::floor(iPoint.x)) & 255;
    int y = ((int) std::floor(iPoint.y)) & 255;
    int z = ((int) std::floor(iPoint.z)) & 255;
    this->order(iPoint);
    //取得在超方体的中的四个顶点（这里是三维的情况下）
    glm::dvec3 a = glm::dvec3(x, y, z);
    glm::dvec3 b = a + this->order_data[0];
    glm::dvec3 c = b + this->order_data[1];
    glm::dvec3 d = c + this->order_data[2];
    //获取单体下的四个方向向量
    glm::dvec3 d0 = point - this->reverseMesh(a);
    int hash1 = permutationTable[permutationTable[permutationTable[x] + y] + z];
    glm::dvec3 d1 = point - this->reverseMesh(b);
    int hash2 = permutationTable[permutationTable[permutationTable[(int)b.x] + (int)b.y] + (int)b.z];
    glm::dvec3 d2 = point - this->reverseMesh(c);
    int hash3 = permutationTable[permutationTable[permutationTable[(int)c.x] + (int)c.y] + (int)c.z];
    glm::dvec3 d3 = point - this->reverseMesh(d);
    int hash4 = permutationTable[permutationTable[permutationTable[(int)d.x] + (int)d.y] + (int)d.z];
    std::cout << d0.x << "   " << d1.x << "   " << d2.x << "   " << d3.x << "   " << std::endl;
    //获取每个向量的梯度向量
    glm::dvec3 g0 = this->getGradient(hash1, a);
    glm::dvec3 g1 = this->getGradient(hash2, b);
    glm::dvec3 g2 = this->getGradient(hash3, c);
    glm::dvec3 g3 = this->getGradient(hash4, c);
    std::cout << g0.x << "   " << g1.x << "   " << g2.x << "   " << g3.x << "   " << std::endl;
    double returnData = this->contribution(d0, d1, d2, d3, g0, g1, g2, g3);
    
    return returnData;
}
//三次线性混合
double SimplexNoise::lerp(double a, double b, double c){
    return 0.0;
}
//排列大小
void SimplexNoise::order(glm::dvec3 point){
    double a = point.x;
    double b = point.y;
    double c = point.z;
    if(a >= b && a >= c){
        if(c >= b){
            glm::dvec3 t = this->order_data[1];
            this->order_data[1] = this->order_data[2];
            this->order_data[2] = t;
        }
    }
    if(b >= a && b >= c){
        glm::dvec3 t = this->order_data[0];
        this->order_data[0] = this->order_data[1];
        this->order_data[1] = t;
        if(c >= a){
            this->order_data[1] = this->order_data[2];
            this->order_data[2] = t;
        }
    }
    if(c >= b && c >= a){
        glm::dvec3 t = this->order_data[0];
        this->order_data[0] = this->order_data[2];
        this->order_data[2] = t;
        if(a >= b){
            this->order_data[2] = this->order_data[1];
            this->order_data[1] = t;
        }
    }
}

//噪声贡献值计算
double SimplexNoise::contribution(glm::dvec3 a, glm::dvec3 b, glm::dvec3 c, glm::dvec3 d, glm::dvec3 g1, glm::dvec3 g2, glm::dvec3 g3, glm::dvec3 g4){
    double r_2 = 0.5;
    glm::dvec4 dist_2 = glm::dvec4(glm::dvec4(glm::dot(a, a), glm::dot(b, b), glm::dot(c, c), glm::dot(d, d)));
    glm::dvec4 dot_data = glm::dvec4(glm::dot(a, g1), glm::dot(b, g2), glm::dot(c, g3), glm::dot(d, g4));
    glm::dvec4 contributionData = glm::max(glm::dvec4(std::abs(r_2 - dist_2.x), std::abs(r_2 - dist_2.y), std::abs(r_2 - dist_2.z), std::abs(r_2 - dist_2.w)), glm::dvec4(0.0)) * dot_data;
    std::cout << contributionData.x << "  " << contributionData.y << "   " << contributionData.z << "    " << contributionData.z  <<std::endl;
    glm::dvec4 expanding_data(31.32);
    double data = glm::dot(expanding_data, contributionData);
    return data;
}

//从单形转换到超方体晶格中
glm::dvec3 SimplexNoise::transformMesh(glm::dvec3 p){
    p.x = p.x + (p.x + p.y + p.z) * K1;
    p.y = p.y + (p.x + p.y + p.z) * K1;
    p.z = p.z + (p.x + p.y + p.z) * K1;
    return p;
}

//从超方格晶格转换到单形之中
glm::dvec3 SimplexNoise::reverseMesh(glm::dvec3 p){
    p.x = p.x - (p.x + p.y + p.z) * K2;
    p.y = p.y - (p.x + p.y + p.z) * K2;
    p.z = p.z - (p.x + p.y + p.z) * K2;
    return p;
}
//全排列数组生成
void SimplexNoise::generatePerumition(){
    for(unsigned int i = 0; i < this->g_Size * 2; ++i){
        this->permutationTable[i] = this->permutation[i % 256];
    }
}
//获取对应单形顶点的梯度向量
glm::dvec3 SimplexNoise::getGradient(int hash, glm::dvec3 p){
    switch(hash & 0xF){
        case 0x0:
            return this->gradientData[0];
        case 0x1:
            return this->gradientData[1];
        case 0x2:
            return this->gradientData[2];
        case 0x3:
            return this->gradientData[3];
        case 0x4:
            return this->gradientData[4];
        case 0x5:
            return this->gradientData[5];
        case 0x6:
            return this->gradientData[6];
        case 0x7:
            return this->gradientData[7];
        case 0x8:
            return this->gradientData[8];
        case 0x9:
            return this->gradientData[9];
        case 0xA:
            return this->gradientData[10];
        case 0xB:
            return this->gradientData[11];
        case 0xC:
            return  this->gradientData[0];
        case 0xD:
            return this->gradientData[9];
        case 0xE:
            return this->gradientData[1];
        case 0xF:
            return this->gradientData[11];
    }
    return glm::dvec3(0.0);
}


void SimplexNoise::test(){
    std::ofstream newFile("./resources/noise/simplex.ppm");
    const uint32_t width = 400, height = 400;
    newFile << "P3\n" << width << " " << height << "\n255\n";
    for(unsigned int i = 0; i < 25; ++i){
        for(unsigned int j = 0; j < 25; ++j){
            glm::dvec3 p(i, j, 7.89101112131415);
            double sum = this->handleData(p);
            std::cout << sum << std::endl;
            int b = (sum + 1) * 255.0 / 2.0;
            newFile << b << " " << b << " " << b << std::endl;
        }
    }
    newFile.close();
    std::cout << "Write Success" << std::endl;
}
