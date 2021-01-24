//
//  resoureces_use.hpp
//  opengl
//
//  Created by adsionli on 2021/1/10.
//  Copyright © 2021 adsionli. All rights reserved.
//

#ifndef resoureces_use_hpp
#define resoureces_use_hpp

#include <stdio.h>

#include <map>
#include <string>
#include "shaders.h"
#include "texture.hpp"
class ResourcesUse{
public:
    static std::map<std::string, Shader> Shaders;
    static std::map<std::string, Texture2D> Textures;
    
    static Shader LoadShader(const char* VShaderFile, const char* FShaderFile, const char* GShaderFile, std::string name);
    static Texture2D LoadTexture(const char* file, bool alpha, std::string name);
    static Shader GetShader(std::string name);
    static Texture2D GetTexture(std::string name);
    
    static void Clear();
private:
    ResourcesUse(){};
    static Shader loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);
    static Texture2D loadTextureFromFile(const char* file, bool alpha);
};


#endif /* resoureces_use_hpp */
