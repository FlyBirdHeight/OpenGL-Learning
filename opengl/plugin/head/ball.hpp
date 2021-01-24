//
//  ball.hpp
//  opengl
//
//  Created by adsionli on 2021/1/20.
//  Copyright © 2021 adsionli. All rights reserved.
//

#ifndef ball_hpp
#define ball_hpp

#include "game_object.hpp"
class BallObj : public GameObject{
public:
    float Radius;
    bool Stuck;
    bool Sticky;
    bool PassThrough;
    BallObj();
    BallObj(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D ballSprite);
    
    glm::vec2 Move(float dt, unsigned int window_width);
    
    void Reset(glm::vec2 position, glm::vec2 velocity);
    
private:
};
#endif /* ball_hpp */
