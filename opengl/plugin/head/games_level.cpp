//
//  games_level.cpp
//  opengl
//
//  Created by adsionli on 2021/1/18.
//  Copyright © 2021 adsionli. All rights reserved.
//

#include "games_level.hpp"
#include <string>
#include <fstream>
#include <sstream>
void GameLevel::Load(const char *file, unsigned int levelWidth, unsigned int levelHeight){
    this->Bricks.clear();
    unsigned int tileCode;
    GameLevel level;
    std::string line;
    //文件流
    std::ifstream fstream(file);
    std::vector<std::vector<unsigned int>> tileData;
    if(fstream){
        while (std::getline(fstream, line)) {
            //字符流，读取文件的行
            std::istringstream sstream(line);
            std::vector<unsigned int> row;
            // 读取被空格分隔的每个数字
            while (sstream >> tileCode) {
                row.push_back(tileCode);
            }
            tileData.push_back(row);
        }
        if (tileData.size() > 0) {
            this->init(tileData, levelWidth, levelHeight);
        }
    }
}

void GameLevel::Draw(SpriteRender &render){
    for (GameObject &tile : this->Bricks){
        if (!tile.Destoryed){
            tile.Draw(render);
        }
    }
}

bool GameLevel::IsCompleted(){
    for(GameObject &tile : this->Bricks){
        if(!tile.Destoryed && !tile.IsSolid){
            return false;
        }
    }
    
    return true;
}

void GameLevel::init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight){
    unsigned int height = static_cast<int>(tileData.size());
    unsigned int width = static_cast<int>(tileData[0].size());
    //单个砖块的长度
    float single_width = levelWidth / static_cast<float>(width);
    //单个砖块的高度
    float single_height = levelHeight / static_cast<float>(height);
    for(unsigned int i = 0; i < height; ++i){
        for(unsigned int j = 0; j < width; ++j){
            if(tileData[i][j] == 1){
                glm::vec2 pos = glm::vec2(single_width * j, single_height * i);
                glm::vec2 size = glm::vec2(single_width, single_height);
                GameObject obj(pos, size, ResourcesUse::GetTexture("block_solid"), glm::vec3(0.8f, 0.8f, 0.7f));
                obj.IsSolid = true;
                this->Bricks.push_back(obj);
            }else if (tileData[i][j] > 1){
                glm::vec3 color = glm::vec3(1.0f); // 默认为白色
                if (tileData[i][j] == 2)
                    color = glm::vec3(0.2f, 0.6f, 1.0f);
                else if (tileData[i][j] == 3)
                    color = glm::vec3(0.0f, 0.7f, 0.0f);
                else if (tileData[i][j] == 4)
                    color = glm::vec3(0.8f, 0.8f, 0.4f);
                else if (tileData[i][j] == 5)
                    color = glm::vec3(1.0f, 0.5f, 0.0f);
                glm::vec2 pos = glm::vec2(single_width * j, single_height * i);
                glm::vec2 size = glm::vec2(single_width, single_height);
                GameObject obj(pos, size, ResourcesUse::GetTexture("block"), color);
                this->Bricks.push_back(obj);
            }
        }
    }
}
