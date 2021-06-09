//
//  dda.cpp
//  opengl
//
//  Created by adsionli on 2021/6/8.
//  Copyright © 2021 adsionli. All rights reserved.
//
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <vector>

struct Coordinate{
    int x;
    int y;
    Coordinate(int xN, int yN){
        x = xN;
        y = yN;
    }
};

static int roundN(const float a){
    return int(a + 0.5);
}

std::vector<Coordinate> lineDDA(int x0, int y0, int xEnd, int yEnd){
    int dx = (x0 - xEnd), dy = (y0 - yEnd);
    int step, k;
    int x = x0, y = y0;
    std::vector<Coordinate> coordinates;
    float xIncrement, yIncrement;
    
    if(fabs(dx) > fabs(dy)){
        step = dx;
    }else{
        step = dy;
    }
    xIncrement = dx / step;
    yIncrement = dy / step;
    coordinates.push_back(Coordinate(x, y));
    for(k = 0; k < step; k++){
        x += xIncrement;
        y += yIncrement;
        coordinates.push_back(Coordinate(x, y));
    }
    
    return  coordinates;
}
