//
//  window_show.cpp
//  test
//
//  Created by adsionli on 2020/7/30.
//  Copyright © 2020 adsionli. All rights reserved.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <string>
//这个回调函数主要使用在视口被用户改变时执行
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//回调函数实际执行过程，就是重新设置视口属性
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
//判断在窗口中是否存在键盘输入
void processInput(GLFWwindow *window)
{
    //glfwGetKey -> 判断是否存在键盘输入事件 GLFW_KEY_ESCAPE -> 输入为esc
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        //glfwSetWindowShouldClose -> 指定关闭相应窗口
        glfwSetWindowShouldClose(window, true);
    }
}
class WindowSho{
public:
    void init(){
        glfwInit();
        //主版本号
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        //副版本号
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        //profile opengl使用模式core profile核心模式
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        //forward_compat向前兼容
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        //双缓冲设置
        glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
         //创建glfw窗口，第一个参数宽度，第二个参数高度，第三个参数是窗口名称，glfwCreateWindow返回的是GLFWwindow对象
         GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
         if (window == NULL)
         {
             printf("创建成功");
             glfwTerminate();
         }
         printf("创建成功");
         glfwMakeContextCurrent(window);
         //glad用来管理opengl中的函数指针
         if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
         {
             printf("Failed to initialize GLAD");
         }
         //设置视口的大小，前两个参数控制窗口左下角的位置。第三个和第四个参数控制渲染窗口的宽度和高度（像素）
         glViewport(0, 0, 800, 600);
         //此函数是为了告诉glfw，每当窗口调整大小的时候调用这个函数。
         glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
         
         /*
          循环渲染，使glfw在被主动关闭之前一直保持运行
          glfwWindowShouldClose函数在我们每次循环的开始前检查一次GLFW是否被要求退出，如果是的话该函数返回true然后渲染循环便结束了，之后为我们就可以关闭应用程序了。
          glfwPollEvents函数检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数（可以通过回调方法手动设置）。
          glfwSwapBuffers函数会交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲），它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上。
          */
         while(!glfwWindowShouldClose(window))
         {
             //加入注册事件(键盘输入esc事件)来主动终止循环渲染
             processInput(window);
             /*
              渲染指令在循环渲染中生效
              glClearColor函数是一个状态设置函数，而glClear函数则是一个状态使用的函数，它使用了当前的状态来获取应该清除为的颜色。
              glClearColor设置清空屏幕所使用的颜色
              glClear函数来清空屏幕的颜色缓冲，可以指定接受一个缓冲位(Buffer Bit)来指定要清空的缓冲，可能的缓冲位有GL_COLOR_BUFFER_BIT，GL_DEPTH_BUFFER_BIT和GL_STENCIL_BUFFER_BIT
              */
             glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
             glClear(GL_COLOR_BUFFER_BIT);
             //检查并调用事件，交换缓冲
             glfwPollEvents();
             glfwSwapBuffers(window);
         }
         //glfwTerminate释放所有已分配资源
         glfwTerminate();
    };
};

