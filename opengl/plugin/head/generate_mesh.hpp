//
//  generate_mesh.hpp
//  opengl
//
//  Created by adsionli on 2021/3/16.
//  Copyright © 2021 adsionli. All rights reserved.
//

#ifndef generate_mesh_hpp
#define generate_mesh_hpp
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <random>
#include <vector>
#include <fstream>
#include <math.h>
#include "perlin.hpp"
#include "camera.h"
#include "shaders.h"
class GenerateMesh{
public:
    PerlinNoise perlin;
    GenerateMesh();
    void createWindow();
    void init(GLFWwindow *window);
    void render(GLFWwindow *window, std::vector<unsigned int> mapChunkVao);
    void initTest();
    void testData();
private:
    std::vector<float> generate_noises(int xOffset, int yOffset);
    std::vector<unsigned int> generate_indices();
    std::vector<float> generate_vertices(int xOffset, int yOffset, std::vector<float> noises);
    void generateMap(unsigned int &mapVao, std::vector<float> vertices);
};

#endif /* generate_mesh_hpp */
