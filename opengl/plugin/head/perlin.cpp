//
//  perlin.cpp
//  opengl
//
//  Created by adsionli on 2021/3/4.
//  Copyright © 2021 adsionli. All rights reserved.
//

#include "perlin.hpp"
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
//    Shader shader("./shader/vs/noise/perlin.vs","./shader/fs/noise/perlin.fs");
//    shader.use();
    
    while (!glfwWindowShouldClose(window)) {
        //float currentTime = glfwGetTime();
        
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glfwSwapBuffers(window);
    }
    
    glfwDestroyWindow(window);
 
    glfwTerminate();
}
//创建晶格
void PerlinNoise::generateMesh(){
    unsigned seed = 2016;
    std::mt19937 generator(seed);
    std::uniform_real_distribution<> distribution{0.0, 1.0};
    auto dice = std::bind(distribution, generator);
    float gradientLen2 = 0.0;
    for (unsigned i = 0; i < this->g_tableSize; ++i) {
        this->gradient[i] = glm::vec2(2 * dice() - 1, 2 * dice() - 1);
        gradientLen2 = this->gradient[i].length();
        this->gradient[i] /= sqrtf(gradientLen2); // normalize gradient
        this->permutationTable[i] = i;
    }
    std::uniform_int_distribution<> distributionInt;
    auto diceInt = std::bind(distributionInt, generator);
    // 创建 permutation 表
    for (unsigned i = 0; i < this->g_tableSize; ++i)
        std::swap(this->permutationTable[i], this->permutationTable[diceInt() & this->p_tableSize]);
    // 扩大表的范围至 [256:512]
    for (unsigned i = 0; i < this->g_tableSize; ++i) {
        this->permutationTable[this->g_tableSize + i] = permutationTable[i];
    }
    
    for(unsigned i = 0; i < this->g_tableSize * 2; ++i){
        std::cout<< this->permutationTable[i] << std::endl;
    }

}

int PerlinNoise::hash(const int &x, const int &y) const
{
    return permutationTable[permutationTable[x] + y] ;
}


void PerlinNoise::testHash(){
    for(unsigned i = 0; i < this->g_tableSize; i++){
        std::cout << this->hash(i, i+1) << std::endl;
    }
}
