//
//  init_window.hpp
//  opengl
//  初始化窗口头文件
//  Created by adsionli on 2021/1/7.
//  Copyright © 2021 adsionli. All rights reserved.
//

#ifndef init_window_hpp
#define init_window_hpp

#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
class InitWindow{
public:
    //每一帧渲染时，清除上一帧的缓冲
    void render(float r, float g, float b, float a);
    void processInput(GLFWwindow* window);
    //初始化窗口
    GLFWwindow* init();
    unsigned int width;
    unsigned int height;
    const char* title;
private:
};

#endif /* init_window_hpp */
