//
//  games.cpp
//  opengl
//
//  Created by adsionli on 2021/1/10.
//  Copyright © 2021 adsionli. All rights reserved.
//

#include "games.hpp"

SpriteRender *Renderer;
GameObject *Player;
BallObj *Ball;
Particles *Particle;
PostProcess *Effects;

const glm::vec2 PLAYER_SIZE(100,20);
const float PLAYER_VELOCITY(500.0f);
const glm::vec2 INITAL_BALL_VELOCITY(100.0f, -350.0f);
const float BALL_RADIUS = 12.5;
float ShakeTime = 0.0f;
Direction VectorDirection(glm::vec2 target);

Games::Games(unsigned int width, unsigned int height)
    : state(GAME_ACTIVE), Keys(), width(width), height(height)
{

}

Games::~Games()
{
    delete Renderer;
    delete Player;
    delete Ball;
    delete Particle;
    delete Effects;
}

void Games::Init(){
    ResourcesUse::LoadShader("./shader/vs/combat/sprite/sprite.vs", "./shader/fs/combat/sprite/sprite.fs", nullptr, "sprite");
    ResourcesUse::LoadShader("./shader/vs/combat/particle/particle.vs", "./shader/fs/combat/particle/particle.fs", nullptr, "particle");
    ResourcesUse::LoadShader("./shader/vs/combat/postprocess/postprocess.vs", "./shader/fs/combat/postprocess/postprocess.fs", nullptr, "postprocess");
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->width), static_cast<float>(this->height), 0.0f, -1.0f, 1.0f);
    //精灵渲染
    Shader sprite = ResourcesUse::GetShader("sprite");
    sprite.use();
    sprite.setMat4("projection", projection);
    sprite.setInt("image", 0);
    //粒子渲染
    Shader particleShader = ResourcesUse::GetShader("particle");
    particleShader.use();
    particleShader.setMat4("projection", projection);
    particleShader.setInt("sprite", 0);
    //后期处理设置
//    Shader processShader = ResourcesUse::GetShader("postprocess");
    
    ResourcesUse::LoadTexture("./resources/images/background.jpg", false, "background");
    ResourcesUse::LoadTexture("./resources/images/paddle.png", true, "paddle");
    ResourcesUse::LoadTexture("./resources/images/block_solid.png", false, "block_solid");
    ResourcesUse::LoadTexture("./resources/images/block.png", false, "block");
    ResourcesUse::LoadTexture("./resources/images/ball_face.png", true, "ball");
    ResourcesUse::LoadTexture("./resources/images/particle.png", true, "particle");
    
    Renderer = new SpriteRender(sprite);
    Particle = new Particles(particleShader, ResourcesUse::GetTexture("particle"), 500);
    Effects = new PostProcess(ResourcesUse::GetShader("postprocess"), this->width * 2, this->height * 2);
    
    GameLevel first,second,third,forth;
    first.Load("./resources/combat/first.combat", this->width, this->height * 0.5);
    second.Load("./resources/combat/second.combat", this->width, this->height * 0.5);
    third.Load("./resources/combat/third.combat", this->width, this->height * 0.5);
    forth.Load("./resources/combat/forth.combat", this->width, this->height * 0.5);
    this->Levels.push_back(first);
    this->Levels.push_back(second);
    this->Levels.push_back(third);
    this->Levels.push_back(forth);
    this->Level = 0;
    
    glm::vec2 playerPos = glm::vec2(this->width / 2.0f - PLAYER_SIZE.x / 2.0f, this->height - PLAYER_SIZE.y);
    Player = new GameObject(playerPos, PLAYER_SIZE, ResourcesUse::GetTexture("paddle"));
    glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
    Ball = new BallObj(ballPos, BALL_RADIUS, INITAL_BALL_VELOCITY, ResourcesUse::GetTexture("ball"));
    
}

void Games::ProcessInput(float dt){
    if(this->state == GAME_ACTIVE){
        float velocity = PLAYER_VELOCITY * dt;
        if (this->Keys[GLFW_KEY_LEFT])
        {
            if (Player->Position.x >= 0){
                Player->Position.x -= velocity;
                if(Ball->Stuck){
                    Ball->Position.x -= velocity;
                }
            }
        }
        if (this->Keys[GLFW_KEY_RIGHT])
        {
            if (Player->Position.x <= this->width - Player->Size.x){
                Player->Position.x += velocity;
                if(Ball->Stuck){
                    Ball->Position.x += velocity;
                }
            }
        }
        if(this->Keys[GLFW_KEY_SPACE]){
            Ball->Stuck = false;
        }
    }
}

void Games::Update(float dt){
    Ball->Move(dt, this->width);
    this->DoCollision();
    Particle->Update(dt, *Ball, 2, glm::vec2(Ball->Radius / 2.0f));
    if(Ball->Position.y > this->height){
        this->ResetRender();
        this->ResetPlayer();
    }
    if (ShakeTime > 0.0f)
    {
        ShakeTime -= dt;
        if (ShakeTime <= 0.0f)
            Effects->Shake = false;
    }
}

void Games::ResetRender(){
    if (this->Level == 0)
       this->Levels[0].Load("./resources/combat/first.combat", this->width, this->height / 2);
   else if (this->Level == 1)
       this->Levels[1].Load("./resources/combat/second.combat", this->width, this->height / 2);
   else if (this->Level == 2)
       this->Levels[2].Load("./resources/combat/third.combat", this->width, this->height / 2);
   else if (this->Level == 3)
       this->Levels[3].Load("./resources/combat/forth.combat", this->width, this->height / 2);
}

void Games::ResetPlayer(){
    Player->Size = PLAYER_SIZE;
    Player->Position = glm::vec2(this->width / 2.0f - PLAYER_SIZE.x / 2.0f, this->height - PLAYER_SIZE.y);
    Ball->Reset(Player->Position + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -(BALL_RADIUS * 2.0f)), INITAL_BALL_VELOCITY);
    // also disable all active powerups
    Effects->Chaos = Effects->Confuse = false;
    Ball->PassThrough = Ball->Sticky = false;
    Player->Color = glm::vec3(1.0f);
    Ball->Color = glm::vec3(1.0f);
}

void Games::Render(){
    Texture2D background = ResourcesUse::GetTexture("background");
    if(this->state == GAME_ACTIVE){
        Effects->BeginRender();
        Renderer->DrawSprite(background, glm::vec2(0.0f, 0.0f), glm::vec2(this->width, this->height), 0.0f);
        this->Levels[this->Level].Draw(*Renderer);
        Player->Draw(*Renderer);
        Particle->Draw();
        Ball->Draw(*Renderer);
        Effects->EndRender();
        Effects->Render(glfwGetTime());
    }
}
//判断小球是否碰撞
Collision Games::CheckCollision(BallObj &one, GameObject &two){
    glm::vec2 center = glm::vec2(one.Position + one.Radius);
    glm::vec2 aabb_half_extents = glm::vec2(two.Size.x / 2, two.Size.y / 2);
    glm::vec2 aabb_center = glm::vec2(two.Position.x + aabb_half_extents.x, two.Position.y + aabb_half_extents.y);
    
    glm::vec2 difference = center - aabb_center;
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    
    glm::vec2 closest = aabb_center + clamped;
        
    difference = center - closest;
    if(glm::length(difference) < one.Radius){
        return std::make_tuple(true, VectorDirection(difference), difference);
    }else{
        return std::make_tuple(false, UP, glm::vec2(0.0f, 0.0f));
    }
}

void Games::DoCollision(){
    for(GameObject &box : this->Levels[this->Level].Bricks){
        if(!box.Destoryed){
            Collision collision = this->CheckCollision(*Ball, box);
            if(std::get<0>(collision)){
                if(!box.IsSolid){
                    box.Destoryed = true;
                }
                ShakeTime = 0.05f;
                Effects->Shake = true;
                Direction dir = std::get<1>(collision);
                glm::vec2 diff_vector = std::get<2>(collision);
                if(dir == LEFT || dir == RIGHT){
                    Ball->Velocity.x = -Ball->Velocity.x;
                    float penetration = Ball->Radius - std::abs(diff_vector.x);
                    if(dir == LEFT)
                        Ball->Position.x += penetration;
                    else
                        Ball->Position.x -= penetration;
                }else{
                    Ball->Velocity.y = -Ball->Velocity.y;
                    float penetration = Ball->Radius - std::abs(diff_vector.y);
                    if(dir == UP){
                        Ball->Position.y -= penetration;
                    }else{
                        Ball->Position.y += penetration;
                    }
                }
            }
        }
    }
    Collision result = CheckCollision(*Ball, *Player);
    if(!Ball->Stuck && std::get<0>(result)){
        float centerBoard = Player->Position.x + Player->Size.x / 2;
        float distance = (Ball->Position.x + Ball->Radius) - centerBoard;
        float percentage = distance / (Player->Size.x / 2);
        float strengh = 2.0;
        glm::vec2 oldVelocity = Ball->Velocity;
        Ball->Velocity.x = INITAL_BALL_VELOCITY.x * percentage * strengh;
        Ball->Velocity.y = -1 * (Ball->Velocity.y);
        Ball->Velocity = glm::normalize(Ball->Velocity) * glm::length(oldVelocity);
    }
}

Direction VectorDirection(glm::vec2 target){
    glm::vec2 compass[] = {
        //上
        glm::vec2(0.0f, 1.0f),
        //下
        glm::vec2(0.0f, -1.0f),
        //左
        glm::vec2(-1.0f, 0.0f),
        //右
        glm::vec2(1.0, 0.0)
    };
    float max = 0.0f;
    unsigned int best_match = -1;
    for(unsigned int i = 0; i < 4; i++){
        float dot_production = glm::dot(glm::normalize(target), compass[i]);
        if(dot_production > max){
            max = dot_production;
            best_match = i;
        }
    }
    
    return (Direction)best_match;
}


