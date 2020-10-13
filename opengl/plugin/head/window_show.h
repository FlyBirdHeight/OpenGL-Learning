//
//  window_show.h
//  opengl
//
//  Created by adsionli on 2020/8/4.
//  Copyright © 2020 adsionli. All rights reserved.
//

#ifndef window_show_h
#define window_show_h
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <string>
#include <iostream>
class Window{
public:
    void processInput(GLFWwindow *window);
    void init();
};
#endif /* window_show_h */
