//
//  particles.hpp
//  opengl
//
//  Created by adsionli on 2021/1/25.
//  Copyright © 2021 adsionli. All rights reserved.
//

#ifndef particles_hpp
#define particles_hpp
#include "shaders.h"
#include "texture.hpp"
#include "resoureces_use.hpp"
#include "game_object.hpp"
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vector_relational.hpp>
#include <glad/glad.h>
struct Particle{
    glm::vec2 Position, Velocity;
    glm::vec4 Color;
    float Life;
    Particle(): Position(0.0f), Velocity(0.0f), Color(1.0f),Life(0.0f){}
};
class Particles{
public:
    Shader shader;
    Texture2D texture;
    unsigned int amount;
    unsigned int vao;
    std::vector<Particle> particles;
    Particles(Shader shader, Texture2D texture, unsigned int amount);
    //初始化
    void init();
    //渲染
    void Draw();
    //寻找第一个消失的粒子
    unsigned int FirstUnusedParticle();
    //更新消失粒子的状态
    void RespawnParticle(Particle &particle, GameObject &obj, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
    //更新全部粒子状态
    void Update(float dt, GameObject &obj, unsigned int newParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
};

#endif /* particles_hpp */
