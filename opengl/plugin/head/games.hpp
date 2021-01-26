//
//  games.hpp
//  opengl
//
//  Created by adsionli on 2021/1/10.
//  Copyright © 2021 adsionli. All rights reserved.
//

#ifndef games_hpp
#define games_hpp

#include "SpriteRender.hpp"
#include "resoureces_use.hpp"
#include <iostream>
#include "shaders.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "games_level.hpp"
#include "ball.hpp"
#include "particles.hpp"
#include "postprocess.hpp"
#include <vector>
enum GameState{
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};
enum Direction{
    UP,
    DOWN,
    LEFT,
    RIGHT
};
typedef std::tuple<bool, Direction, glm::vec2> Collision;
class Games{
public:
    GameState state;
    bool Keys[1024];
    unsigned int width,height;
    std::vector<GameLevel> Levels;
    unsigned int Level;
    Games(unsigned int scr_width, unsigned int scr_height);
    ~Games();
    //初始化游戏
    void Init();
    //处理输入事件
    void ProcessInput(float dt);
    //游戏运行时更新
    void Update(float dt);
    //游戏渲染
    void Render();
    //碰撞检测
    Collision CheckCollision(BallObj &one, GameObject &two);
    //处理碰撞检测
    void DoCollision();
    //重置渲染
    void ResetRender();
    //重置底部挡板状态
    void ResetPlayer();
};
#endif /* games_hpp */
