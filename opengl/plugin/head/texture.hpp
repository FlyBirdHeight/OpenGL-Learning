//
//  texture.hpp
//  opengl
//
//  Created by adsionli on 2021/1/10.
//  Copyright © 2021 adsionli. All rights reserved.
//

#ifndef texture_hpp
#define texture_hpp
#include <glad/glad.h>
#include <GLFW/glfw3.h>
class Texture2D{
public:
    //纹理高度、宽度
    unsigned int Width,Height;
    //纹理ID
    unsigned int ID;
    //纹理格式
    unsigned int Internal_Format;
    unsigned int Image_Format;
    //纹理边缘处理方式
    unsigned int Wrap_S;
    unsigned int Wrap_T;
    //纹理过滤
    unsigned int Filter_Min;
    unsigned int Filter_Max;
    //纹理加载
    void Generate(unsigned int width, unsigned int height, unsigned char* data);
    //纹理绑定
    void Bind() const;
    Texture2D();
private:
};
#endif
