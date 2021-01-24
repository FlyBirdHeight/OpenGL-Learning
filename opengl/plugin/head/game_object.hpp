//
//  game_object.hpp
//  opengl
//
//  Created by adsionli on 2021/1/18.
//  Copyright © 2021 adsionli. All rights reserved.
//

#ifndef game_object_hpp
#define game_object_hpp

#include <iostream>
#include <glad/glad.h>
#include "texture.hpp"
#include "SpriteRender.hpp"
#include <glm/glm.hpp>
class GameObject{
public:
    //Velocity速度，这里使用的是二维向量，代表是x轴上的相对速度和y轴上的相对速度，正的代表是正方向，负的代表是负方向
    glm::vec2 Position, Size, Velocity;
    glm::vec3 Color;
    float rotation;
    //砖块是否是不可分裂的判断
    bool IsSolid;
    bool Destoryed;
    Texture2D sprite;
    GameObject();
    GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));
    //虚基类，子类中希望被重载
    virtual void Draw(SpriteRender &render);
private:
};
#endif /* game_object_hpp */
