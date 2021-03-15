//
//  perlin.hpp
//  opengl
//
//  Created by adsionli on 2021/3/15.
//  Copyright © 2021 adsionli. All rights reserved.
//

#ifndef perlin_hpp
#define perlin_hpp
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <random>
#include <cmath>
#include <vector>
class PerlinNoise{
public:
    PerlinNoise();
    std::vector<int> permutation;
    double perlin_noise(float x, float y);
    //分型布朗运动
    double fbm(float x, float y);
private:
    //初始化索引数组
    void generate_permutation();
    //获取梯度向量
    double gradient(int permutationIndex, double x, double y, double z);
    //双线性混合
    double lerp(double t, double a, double b);
    //缓和曲线
    double fade(double t);
};
#endif /* perlin_hpp */
