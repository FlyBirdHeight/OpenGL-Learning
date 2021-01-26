//
//  particles.cpp
//  opengl
//
//  Created by adsionli on 2021/1/25.
//  Copyright © 2021 adsionli. All rights reserved.
//

#include "particles.hpp"
Particles::Particles(Shader shader, Texture2D texture, unsigned int amount) : shader(shader), texture(texture), amount(amount){
    this->init();
}

void Particles::init(){
    unsigned int vbo;
    float particlesArray[] = {
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };
    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(this->vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particlesArray), particlesArray, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindVertexArray(0);
    
    for (unsigned int i = 0; i < amount; ++i) {
        this->particles.push_back(Particle());
    }
}

void Particles::Update(float dt, GameObject &obj, unsigned int newParticles, glm::vec2 offset){
    for(int i = 0; i < newParticles; ++i){
        int unusedParticle = this->FirstUnusedParticle();
        this->RespawnParticle(this->particles[unusedParticle], obj, offset);
    }
    
    for(unsigned int i = 0; i < this->amount; ++i){
        Particle &p = this->particles[i];
        p.Life -= dt; // reduce life
        if (p.Life > 0.0f)
        {    // particle is alive, thus update
            p.Position -= p.Velocity * dt;
            p.Color.a -= dt * 2.5f;
        }
    }
}

void Particles::Draw(){
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    this->shader.use();
    for (Particle particle : this->particles)
    {
       if (particle.Life > 0.0f)
       {
           this->shader.setVec2("offset", particle.Position);
           this->shader.setVec4("color", particle.Color);
           this->texture.Bind();
           glBindVertexArray(this->vao);
           glDrawArrays(GL_TRIANGLES, 0, 6);
           glBindVertexArray(0);
       }
    }
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
unsigned int lastUsedParticle = 0;
unsigned int Particles::FirstUnusedParticle(){
    for(unsigned int i = lastUsedParticle; i < this->amount; ++i){
        if(this->particles[i].Life <= 0.0f){
            lastUsedParticle = i;
            return i;
        }
    }
    for (unsigned int i = 0; i < lastUsedParticle; ++i) {
        if(this->particles[i].Life <= 0.0f){
            lastUsedParticle = i;
            return i;
        }
    }
    lastUsedParticle = 0;
    return 0;
}

void Particles::RespawnParticle(Particle &particle, GameObject &obj, glm::vec2 offset){
    float random = ((rand() % 100) - 50) / 10.0f;
    float rColor = 0.5 + ((rand() % 100) / 100.0f);
    particle.Position = obj.Position + random + offset;
    particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
    particle.Life = 1.0f;
    particle.Velocity = obj.Velocity * 0.1f;
}


