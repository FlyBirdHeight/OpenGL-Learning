//
//  worley_noise.cpp
//  opengl
//
//  Created by adsionli on 2021/3/9.
//  Copyright © 2021 adsionli. All rights reserved.
//

#include "worley_noise.hpp"

WorleyNoise::WorleyNoise(){
    this->generatePositionPoint();
}

void WorleyNoise::generatePositionPoint(){
    unsigned int seed = 2016;
    std::mt19937 generateData(seed);
    std::uniform_real_distribution<double> permutition{0.0, 1.0};
    auto dice = std::bind(permutition, generateData);
    float gradientLen2 = 0.0;
    for(unsigned int i = 0; i < g_Size; ++i){
        this->positionData[i] = glm::dvec3(2 * dice() - 1, 2 * dice() - 1, 2 * dice() - 1);
        gradientLen2 = this->positionData[i].length();
        this->positionData[i] /= sqrtf(gradientLen2);
    }
    
    for(unsigned i = 0; i < g_Size; i++){
        this->permutation[256+i] = this->permutation[i];
    }
}

glm::dvec3 WorleyNoise::hash(int x, int y, int z){
    return positionData[permutation[permutation[permutation[z] + y] + x]];
}

double WorleyNoise::getMinDistance(int x, int y, int z, glm::dvec3 p){
    float min_dist = 1.0;
    for(int i = -1; i < 2; ++i){
        for(int j = -1; j < 2; ++j){
            for(int k = -1; k < 2; ++k){
                glm::dvec3 neighborI = glm::dvec3((x + i) & 256, (y + j) & 256, (z + k) & 256);
                glm::dvec3 neighborPos = glm::dvec3(std::floor(p.x) + i, std::floor(p.y) + j, std::floor(p.z) + k);
                glm::dvec3 point = neighborPos + this->hash(neighborI.x, neighborI.y, neighborI.z);
                double dist = glm::distance(p, point);
                min_dist = fmin(min_dist, dist);
            }
        }
    }
    return min_dist;
}

double WorleyNoise::handleData(glm::dvec3 p){
    int xi = (int)std::floor(p.x) & g_Size;
    int yi = (int)std::floor(p.y) & g_Size;
    int zi = (int)std::floor(p.z) & g_Size;
    
    double returnData = this->getMinDistance(xi, yi, zi, p);
    
    return returnData;
}

double WorleyNoise::fbm1(double octaves, double x, double y, double z){
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

void WorleyNoise::test(){
    std::ofstream newFile("./resources/noise/worley_noise2.ppm");
    const uint32_t width = 400, height = 400;
    newFile << "P3\n" << width << " " << height << "\n255\n";
    for(double i = 0; i < 25.0; i += 0.05){
        for(double j = 0; j < 25.0; j += 0.05){
            for(double k = 0; k < 1.0; k += 0.05){
                double sum = this->fbm1(5, i, j, k);
                int b = (sum + 1) * 255.0 / 2.0;

                newFile << b << " " << b << " " << b << std::endl;
            }
        }
    }
    newFile.close();
    std::cout << "Write Success" << std::endl;
}
