//
//  triangle.h
//  opengl
//
//  Created by adsionli on 2020/8/4.
//  Copyright © 2020 adsionli. All rights reserved.
//

#ifndef triangle_h
#define triangle_h
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <string>
#include <iostream>

class Triangle
{
    public:
        //编译GLSL顶点着色器
        const char *vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";
        const char *fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\n\0";
        //判断在窗口中是否存在键盘输入
        void processInput(GLFWwindow *window);
        void setVertex(float data[]);
        void create();
    protected:
        unsigned int EBO;
        unsigned int VBO;
        unsigned int vertexShader;
        unsigned int VAO;
        float *vertices;
};




#endif /* triangle_h */
