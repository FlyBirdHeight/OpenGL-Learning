//
//  text_draw.cpp
//  opengl
//  绘制中文文字实现类
//  Created by adsionli on 2021/1/7.
//  Copyright © 2021 adsionli. All rights reserved.
//

#include "text_draw.hpp"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <utility>
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
void TextDraw::work(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, title, NULL, NULL);
    glfwMakeContextCurrent(window);
//    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    wchar_t drawText[] = L"李景秋浙江工业大学傅琇清皮豆天下无敌世界第一二三四五六七八九十百千方鑫陈启鹏";

    Shader shader(fontVs, fontFs);
    glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
    shader.use();
    shader.setMat4("projection", projection);
    shader.setInt("fontTexture", 0);
    TextureFind textureFind(fontPath, 16);
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glDisable(GL_DEPTH_TEST);
        textureFind.drawText(10, 30, drawText, shader);
        
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
}
