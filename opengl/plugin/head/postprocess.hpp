//
//  postprocess.hpp
//  opengl
//
//  Created by adsionli on 2021/1/26.
//  Copyright © 2021 adsionli. All rights reserved.
//

#ifndef postprocess_hpp
#define postprocess_hpp
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shaders.h"
#include "texture.hpp"
class PostProcess{
public:
    Shader PostProcessShader;
    Texture2D PostProcessTexture;
    unsigned int Width,Height;
    bool Chaos, Shake, Confuse;
    PostProcess(Shader shader, unsigned int width, unsigned int height);
    void BeginRender();
    void EndRender();
    void Render(float dt);
private:
    unsigned int vao;
    unsigned int FBO, MSFBO;
    unsigned int RBO;
    
    void initRenderData();
};

#endif /* postprocess_hpp */
