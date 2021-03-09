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
    //单形顶点
    glm::dvec3 ac = this->reverseMesh(a);
    glm::dvec3 bc = this->reverseMesh(b);
    glm::dvec3 cc = this->reverseMesh(c);
    glm::dvec3 dc = this->reverseMesh(d);
    //获取单体下的四个方向向量
    glm::dvec3 d0 = point - ac;
    int hash1 = permutationTable[permutationTable[permutationTable[x] + y] + z];
    glm::dvec3 d1 = point - bc;
    int hash2 = permutationTable[permutationTable[permutationTable[(int)b.x] + (int)b.y] + (int)b.z];
    glm::dvec3 d2 = point - cc;
    int hash3 = permutationTable[permutationTable[permutationTable[(int)c.x] + (int)c.y] + (int)c.z];
    glm::dvec3 d3 = point - dc;
    int hash4 = permutationTable[permutationTable[permutationTable[(int)d.x] + (int)d.y] + (int)d.z];
    //获取每个向量的梯度向量
    glm::dvec3 g0 = this->getGradient(hash1, a);
    glm::dvec3 g1 = this->getGradient(hash2, b);
    glm::dvec3 g2 = this->getGradient(hash3, c);
    glm::dvec3 g3 = this->getGradient(hash4, c);

    double returnData = this->contribution(d0, d1, d2, d3, g0, g1, g2, g3);
    
    return returnData;
}
//双线性混合
double SimplexNoise::lerp(double a, double b, double c){
    
    return a * (1 - c) + b * c;
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
    double r_2 = 0.6;
    double n0,n1,n2,n3;
    double t0 = r_2 - a.x * a.x - a.y * a.y - a.z * a.z;
    double t1 = r_2 - b.x * b.x - b.y * b.y - b.z * b.z;
    double t2 = r_2 - c.x * c.x - c.y * c.y - c.z * c.z;
    double t3 = r_2 - d.x * d.x - d.y * d.y - d.z * d.z;
    if(t0 < 0){
        n0 = 0;
    }else{
        t0 *= t0;
        n0 = t0 * t0 * (g1.x + g1.y + g1.z);
    }
    
    if(t1 < 0){
        n1 = 0;
    }else{
        t1 *= t1;
        n1 = t1 * t1 * (g2.x + g2.y + g3.z);
    }
    
    if(t2 < 0){
        n2 = 0;
    }else{
        t2 *= t2;
        n2 = t2 * t2 * (g3.x + g3.y + g3.z);
    }
    
    if(t0 < 0){
        n3 = 0;
    }else{
        t3 *= t3;
        n3 = t3 * t3 * (g4.x + g4.y + g4.z);
    }
    
    return 31.316 * (n0 + n1 + n2 + n3);
}

//从单形转换到超方体晶格中
glm::dvec3 SimplexNoise::transformMesh(glm::dvec3 p){
    p = p + glm::dot(p, glm::dvec3(K1));
    
    return p;
}

//从超方格晶格转换到单形之中
glm::dvec3 SimplexNoise::reverseMesh(glm::dvec3 p){
    p = p - glm::dot(p, glm::dvec3(K2));
    
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
double SimplexNoise::fbm1(double octaves, double x, double y, double z){
    double output = .0;
    double denom = .0;
    double frequency = 1.;
    double amplitude = 1.;
    
    for(double i = 0; i < octaves; i++){
        output += amplitude * this->handleData(glm::dvec3(x * frequency, y * frequency, z * frequency));
        denom += amplitude;
        
        frequency *= 2.0;
        amplitude *= 0.5;
    }
    
    return output / denom;
}

double SimplexNoise::fbm2(double octaves, double x, double y, double z){
    double output = 0.0;
    double frequency = 1.0;
    double amplitude = 1.0;
    
    for(double i = 0; i < octaves; i++){
        output += amplitude * std::abs(this->handleData(glm::dvec3(x * frequency, y * frequency, z * frequency)));
        
        frequency *= 2.0;
        amplitude *= 0.5;
    }
    
    output = sin(output + x * frequency / 16.0);

    return output;
}

void SimplexNoise::test(){
    std::ofstream newFile("./resources/noise/simplex_fbm5.ppm");
    const uint32_t width = 400, height = 400;
    newFile << "P3\n" << width << " " << height << "\n255\n";
    for(double i = 0; i < 25.0; i += 0.05){
        for(double j = 0; j < 25.0; j += 0.05){
            for(double k = 0; k < 1.0; k += 0.05){
                double sum = this->fbm2(5.0, i, j, k);
                int b = (sum + 1) * 255.0 / 2.0;

                newFile << b << " " << b << " " << b << std::endl;
            }
        }
    }
    newFile.close();
    std::cout << "Write Success" << std::endl;
}
