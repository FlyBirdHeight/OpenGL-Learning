//
//  SpriteRender.hpp
//  opengl
//
//  Created by adsionli on 2021/1/18.
//  Copyright © 2021 adsionli. All rights reserved.
//

#ifndef SpriteRender_hpp
#define SpriteRender_hpp

#include <iostream>
#include "texture.hpp"
#include "shaders.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class SpriteRender{
public:
    SpriteRender(Shader shader);
    ~SpriteRender();
    void DrawSprite(Texture2D& texture, glm::vec2 position, glm::vec2 size = glm::vec2(10, 10), float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
private:
    Shader shader;
    unsigned int quadVao;
    
    void initRender();
};

#endif /* SpriteRender_hpp */
