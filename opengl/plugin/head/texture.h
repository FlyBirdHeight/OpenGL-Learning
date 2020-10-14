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
#include <math.h>
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
bool firstMouse = true;
float yaw   = -90.0f; 
float pitch =  0.0f;
float lastX =  800.0f / 2.0;
float lastY =  600.0 / 2.0;
float fov   =  45.0f;
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
  //因为y坐标是从底部往顶部依次增大的,所以是反的
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.05;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}
//鼠标滚轮回调
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
  if(fov >= 1.0f && fov <= 45.0f)
    fov -= yoffset;
  if(fov <= 1.0f)
    fov = 1.0f;
  if(fov >= 45.0f)
    fov = 45.0f;
}
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
        //配置焦点获取,主要作用就是无论怎么去移动鼠标，光标都不会显示了，它也不会离开窗口
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
        }
        std::string vsFilePath = "/Users/adsionli/code/c++/opengl/opengl/opengl/plugin/shader/vs/texture_test.vs";
        if(type == "transform"){
            vsFilePath = "/Users/adsionli/code/c++/opengl/opengl/opengl/plugin/shader/vs/transform_test.vs";
        }else if(type == "coordinate"){
            vsFilePath = "/Users/adsionli/code/c++/opengl/opengl/opengl/plugin/shader/vs/coordinate_test.vs";
        }else if(type == "3D"){
            glEnable(GL_DEPTH_TEST);
            glfwSetScrollCallback(window, scroll_callback);
            glfwSetCursorPosCallback(window, mouse_callback);
            vsFilePath = "/Users/adsionli/code/c++/opengl/opengl/opengl/plugin/shader/vs/3d_test.vs";
        }
    
        std::string fsFilePath = "/Users/adsionli/code/c++/opengl/opengl/opengl/plugin/shader/fs/texture_test.fs";
        Shader shader(vsFilePath,fsFilePath);
        glGenVertexArrays(1,&VAO);
        glGenBuffers(1, &VBO);
        if(type != "3D"){
            glGenBuffers(2, &EBO);
        }
        glm::vec3 cubePositions[] = {
          glm::vec3( 0.0f,  0.0f,  0.0f),
          glm::vec3( 2.0f,  5.0f, -15.0f),
          glm::vec3(-1.5f, -2.2f, -2.5f),
          glm::vec3(-3.8f, -2.0f, -12.3f),
          glm::vec3( 2.4f, -0.4f, -3.5f),
          glm::vec3(-1.7f,  3.0f, -7.5f),
          glm::vec3( 1.3f, -2.0f, -2.5f),
          glm::vec3( 1.5f,  2.0f, -2.5f),
          glm::vec3( 1.5f,  0.2f, -1.5f),
          glm::vec3(-1.3f,  1.0f, -1.5f)
        };
        float vertices3D[] = {
           -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
        };
        
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
        
        if(type == "3D"){
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3D), vertices3D, GL_STATIC_DRAW);
            // position attribute
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            // texture coord attribute
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);
        }else{
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
        }
        
         

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
        
        
        if(type == "coordinate"){
            glm::mat4 model = glm::mat4(1.0f);
            glm::mat4 view = glm::mat4(1.0f);
            glm::mat4 projection = glm::mat4(1.0f);
            model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f,0.0f,0.0f));
            view = glm::translate(view, glm::vec3(0.0f,0.0f,-3.0f));
            projection = glm::perspective(glm::radians(45.0f),(float)SCR_WIDTH/(float)SCR_HEIGHT,0.1f,100.0f);
            //注入着色器中
            //模型矩阵，世界空间
            int modelLoc = glGetUniformLocation(shader.ID, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            //观察矩阵，观察空间
            int viewLoc = glGetUniformLocation(shader.ID,"view");
            glUniformMatrix4fv(viewLoc,1,GL_FALSE, glm::value_ptr(view));
            //投影矩阵，裁剪空间
            int projectionLoc = glGetUniformLocation(shader.ID,"projection");
            glUniformMatrix4fv(projectionLoc,1,GL_FALSE, glm::value_ptr(projection));
        }
        
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
            if(type == "3D"){
                float currentFrame = glfwGetTime();
                deltaTime = currentFrame - lastFrame;
                lastFrame = currentFrame;
                //清除深度缓存
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                
                glm::mat4 view = glm::mat4(1.0f);
//                普通设置
//                view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
//                自旋转
//                float radius = 10.0f;
//                float camX = sin(glfwGetTime()) * radius;
//                float camZ = cos(glfwGetTime()) * radius;
//                view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
                //cameraPos + cameraFront，这两个向量相加，可以将目标位置坐标固定，指向z轴负方向。
                view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
                //放入着色器中
                unsigned int viewLoc  = glGetUniformLocation(shader.ID, "view");
                glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
                
                glm::mat4 projection = glm::mat4(1.0f);
                projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
                shader.setMat4("projection", projection);
                
                for (unsigned int i = 0; i < 10; i++)
                {
                    glm::mat4 model = glm::mat4(1.0f);
                    model = glm::translate(model, cubePositions[i]);
                    float angle = 20.0f * i;
                    if(i%3 == 0){
                        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
                    }else{
                        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
                    }
                    
                    shader.setMat4("model", model);

                    glDrawArrays(GL_TRIANGLES, 0, 36);
                }
                
            }else{
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            }
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

protected:
    const unsigned int SCR_WIDTH = 800;
    const unsigned int SCR_HEIGHT = 600;
    glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);

    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
    float deltaTime = 0.0f; // 当前帧与上一帧的时间差
    float lastFrame = 0.0f; // 上一帧的时间

private:
    //初始化部分数据
    void processInput(GLFWwindow *window){
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
            //glfwSetWindowShouldClose -> 指定关闭相应窗口
            glfwSetWindowShouldClose(window, true);
        }
        //设置键盘移动速度
        float cameraSpeed = 2.5f * deltaTime;
        //更新摄像机位置
        //前进，更新z轴
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            cameraPos += cameraSpeed * cameraFront;
        }
        //后退，更新z轴
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
           cameraPos -= cameraSpeed * cameraFront;
        }
        //左移，更新x轴，使用叉乘创建一个右向量，然后减去之后往X轴负方向移动
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
           cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        }
        //右移，更新x轴，使用叉乘创建一个右向量，然后加上之后往X轴正方向移动
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        }
    }
    
};

#endif /* texture_h */
