//
//  ball.cpp
//  opengl
//
//  Created by adsionli on 2021/1/20.
//  Copyright © 2021 adsionli. All rights reserved.
//

#include "ball.hpp"

void BallObj::Reset(glm::vec2 position, glm::vec2 velocity){
    this->Position = position;
    this->Velocity = velocity;
    this->Stuck = true;
    this->Sticky = false;
    this->PassThrough = false;
}

glm::vec2 BallObj::Move(float dt, unsigned int window_width){
//    return glm::vec2(0.0f);
    if(!this->Stuck){
        this->Position += this->Velocity * dt;
        if(this->Position.x <= 0.0f){
            this->Velocity.x = -this->Velocity.x;
            this->Position.x = 0.0f;
        }else if(this->Position.x + this->Size.x >= window_width){
            this->Velocity.x = -this->Velocity.x;
            this->Position.x = window_width - this->Size.x;
        }
        if(this->Position.y <= 0.0f){
            this->Velocity.y = -this->Velocity.y;
            this->Position.y = 0.0f;
        }
    }
    
    return this->Position;
}


BallObj::BallObj()
    : GameObject(), Radius(12.5f), Stuck(true), Sticky(false), PassThrough(false)  { }

BallObj::BallObj(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite)
    : GameObject(pos, glm::vec2(radius * 2.0f, radius * 2.0f), sprite, glm::vec3(1.0f), velocity), Radius(radius), Stuck(true), Sticky(false), PassThrough(false) { }
