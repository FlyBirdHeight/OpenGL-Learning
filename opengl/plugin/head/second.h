//
//  second.h
//  opengl
//
//  Created by adsionli on 2021/1/15.
//  Copyright © 2021 adsionli. All rights reserved.
//

#ifndef second_h
#define second_h
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "games.hpp"
#include "resoureces_use.hpp"
unsigned int SCR_WIDTH = 800, SCR_HEIGHT = 600;
Games Breakout(SCR_WIDTH, SCR_HEIGHT);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
class Second{
public:
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    void init(){
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
//      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        #endif

        GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "break_out second", NULL, NULL);
        //屏幕监视器，查看屏幕个数及相关属性
        glfwMakeContextCurrent(window);
        glfwSetKeyCallback(window, key_callback);

        
        if (window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
        }
        
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
        }
        glEnable(GL_BLEND);
        glEnable(GL_CULL_FACE);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        Breakout.Init();
        
        while(!glfwWindowShouldClose(window)){
            float currentTime = glfwGetTime();
            this->deltaTime = currentTime - this->lastFrame;
            this->lastFrame = currentTime;
            glfwPollEvents();
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            Breakout.ProcessInput(this->deltaTime);
            Breakout.Update(this->deltaTime);
            Breakout.Render();
            
            glfwSwapBuffers(window);
        }
    }
    
private:
    
};
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS){
            Breakout.Keys[key] = true;
        }else if (action == GLFW_RELEASE){
            Breakout.Keys[key] = false;
        }
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
#endif /* second_h */
