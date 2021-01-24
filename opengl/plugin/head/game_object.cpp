//
//  game_object.cpp
//  opengl
//
//  Created by adsionli on 2021/1/18.
//  Copyright © 2021 adsionli. All rights reserved.
//

#include "game_object.hpp"

GameObject::GameObject()
    : Position(0, 0), Size(1, 1), Velocity(0.0f), Color(1.0f), rotation(0.0f), sprite(), IsSolid(false), Destoryed(false) { }

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity)
    : Position(pos), Size(size), Velocity(velocity), Color(color), rotation(0.0f), sprite(sprite), IsSolid(false), Destoryed(false) { }

void GameObject::Draw(SpriteRender &render)
{
    render.DrawSprite(this->sprite, this->Position, this->Size, this->rotation, this->Color);
}
