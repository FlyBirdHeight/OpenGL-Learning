//
//  text_draw.hpp
//  opengl
//  绘制中文文字头文件
//  Created by adsionli on 2021/1/7.
//  Copyright © 2021 adsionli. All rights reserved.
//

#ifndef text_draw_hpp
#define text_draw_hpp
#include "character_set.h"
#include "texture_find.h"
#include "shaders.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>
#include <iostream>
#include <stdio.h>
#pragma execution_character_set("utf-8")

class TextDraw{
public:
    unsigned int SCR_WIDTH = 800;
    unsigned int SCR_HEIGHT = 600;
    const char* title = "中文文本渲染";
    void work();
private:
    const char* fontPath = "./resources/font/default.ttf";
    const char* fontVs = "./shader/vs/combat/font.vs";
    const char* fontFs = "./shader/fs/combat/font.fs";
    unsigned int fontVao,fontVbo;
    void draw();
};



#endif /* text_draw_hpp */
