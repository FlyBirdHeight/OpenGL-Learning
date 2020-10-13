//
//  texture.h
//  opengl
//  纹理类头文件
//  Created by adsionli on 2020/8/4.
//  Copyright © 2020 adsionli. All rights reserved.
//

#ifndef texture_h
#define texture_h
#include "shaders.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#define STB_IMAGE_IMPLEMENTATION    // include之前必须定义
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class TextureUse{
public:
    //对外使用接口
    void init(const std::string& type)
    {
        unsigned int VAO,VBO,EBO;
         glfwInit();
         glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
         glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
         glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
         glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
         GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "纹理使用", NULL, NULL);
         if (window == NULL)
         {
             std::cout << "Failed to create GLFW window" << std::endl;
             glfwTerminate();
         }
         glfwMakeContextCurrent(window);
//         glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

         if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
         {
             std::cout << "Failed to initialize GLAD" << std::endl;
         }
         char *vsFilePath = "/Users/adsionli/code/c++/opengl/opengl/opengl/plugin/shader/vs/texture_test.vs";
        if(type == "transform"){
            vsFilePath = "/Users/adsionli/code/c++/opengl/opengl/opengl/plugin/shader/vs/transform_test.vs";
        }
    
         char *fsFilePath = "/Users/adsionli/code/c++/opengl/opengl/opengl/plugin/shader/fs/texture_test.fs";
         Shader shader(vsFilePath,fsFilePath);
         glGenVertexArrays(1,&VAO);
         glGenBuffers(1, &VBO);
         glGenBuffers(2, &EBO);

         float vertices[] = {
            // positions          // colors           // texture coords
             0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
             0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
         };
         unsigned int indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
         };

         glBindVertexArray(VAO);

         glBindBuffer(GL_ARRAY_BUFFER, VBO);
         glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
         glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

         // 位置采样
         glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
         glEnableVertexAttribArray(0);
         // 颜色采样
         glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
         glEnableVertexAttribArray(1);
         // 纹理采样
         glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
         glEnableVertexAttribArray(2);

         unsigned int texture1, texture2;
         //第一个纹理的生成过程
         glGenTextures(1, &texture1);
         glBindTexture(GL_TEXTURE_2D, texture1);
         // 设置纹理的环绕方式，在x轴的方向上
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
         //设置纹理的环绕方式，在y轴的方向上
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
         //设置缩小过程中的过滤方式，是线性过滤方式
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
         //通过stb_image这个库来加载图片
         int width, height, nrChannels;
         //设置图片的y轴进行翻转
         stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded
         //添加图片数据
         unsigned char *data = stbi_load("/Users/adsionli/code/c++/opengl/opengl/opengl/plugin/resources/images/area.png", &width, &height, &nrChannels, 0);
         if (data)
         {
            //进行纹理生成
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            //设置多级渐远纹理
            glGenerateMipmap(GL_TEXTURE_2D);
         }
         else
         {
            std::cout << "Failed to load texture" << std::endl;
         }
         //释放图像
         stbi_image_free(data);

         //第二个纹理的生成过程
         glGenTextures(1, &texture2);
         glBindTexture(GL_TEXTURE_2D, texture2);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
         data = stbi_load("/Users/adsionli/code/c++/opengl/opengl/opengl/plugin/resources/images/awesomeface.png", &width, &height, &nrChannels, 0);
         if (data)
         {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
         }
         else
         {
            std::cout << "Failed to load texture" << std::endl;
         }
         stbi_image_free(data);
            
        //通过glUniform1i函数设置每个采样器的方式告诉opengl 每个着色器采样器属于哪个纹理单元
        shader.use();
        glUniform1i(glGetUniformLocation(shader.ID,"texture1"),0);//手动设置方式
        shader.setInt("texture2",1);//着色器类设置方式
        
        while (!glfwWindowShouldClose(window))
        {
            processInput(window);
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            //激活纹理
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture1);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texture2);
             
            shader.use();
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            if(type == "transform"){
                //设置旋转平移矩阵
                unsigned int transformLoc = glGetUniformLocation(shader.ID, "transform");
                //创建d单位矩阵
                glm::mat4 trans = glm::mat4(1.0f);
                //设置矩阵平移
                trans = glm::translate(trans, glm::vec3(0.5f,-0.5f,0.0f));
                trans = glm::scale(trans,glm::vec3(0.5f,0.5f,0.5f));
                //设置矩阵随时间旋转
                trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f,0.0f,1.0f));
                //变换矩阵注入着色器中
                glUniformMatrix4fv(transformLoc,1,GL_FALSE,glm::value_ptr(trans));
            }
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
protected:
    const unsigned int SCR_WIDTH = 800;
    const unsigned int SCR_HEIGHT = 600;
    unsigned int EBO;
    unsigned int VBO;
    unsigned int VAO;
private:
    //初始化部分数据
    void processInput(GLFWwindow *window){
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
            //glfwSetWindowShouldClose -> 指定关闭相应窗口
            glfwSetWindowShouldClose(window, true);
        }
    }
};

#endif /* texture_h */
