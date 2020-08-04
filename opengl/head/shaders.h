//
//  shaders.h
//  opengl
//
//  Created by adsionli on 2020/8/4.
//  Copyright © 2020 adsionli. All rights reserved.
//

#ifndef shaders_h
#define shaders_h
//头文件顶部使用了几个预处理指令(Preprocessor Directives)。这些预处理指令会告知你的编译器只在它没被包含过的情况下才包含和编译这个头文件，即使多个文件都包含了这个着色器头文件。它是用来防止链接冲突的。
#include <glad/glad.h> // 包含glad来获取所有的必须OpenGL头文件

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
class Shader
{
public:
    // 构造器读取并构建着色器
    void create(const GLchar* vertexPath, const GLchar* fragmentPath);
    // 使用/激活程序
    void use();
    // uniform工具函数
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
protected:
    // 程序ID
    unsigned int shaderProgram;
};

#endif /* shaders_h */
