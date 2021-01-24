//
//  games_level.hpp
//  opengl
//
//  Created by adsionli on 2021/1/18.
//  Copyright © 2021 adsionli. All rights reserved.
//

#ifndef games_level_hpp
#define games_level_hpp
#include <iostream>
#include <glad/glad.h>
#include "texture.hpp"
#include <vector>
#include "game_object.hpp"
#include "SpriteRender.hpp"
#include "resoureces_use.hpp"
class GameLevel{
public:
    std::vector<GameObject> Bricks;
    GameLevel(){};
    //加载关卡文件
    void Load(const char *file, unsigned int levelWidth, unsigned int levelHeight);
    //渲染关卡
    void Draw(SpriteRender &render);
    //判断关卡是否完成
    bool IsCompleted();
private:
    //根据砖块数据初始化关卡
    void init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight);
};


#endif /* games_level_hpp */
