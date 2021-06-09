//
//  bresenham.cpp
//  opengl
//
//  Created by adsionli on 2021/6/8.
//  Copyright © 2021 adsionli. All rights reserved.
//

#include <stdio.h>

#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <vector>

struct BersenhamPointData {
    int x;
    int y;
    float p;
    BersenhamPointData(int xN, int yN, float pN): x(xN), y(yN), p(pN){
        
    }
};

static std::vector<BersenhamPointData> bersenhamLine(int x0, int y0, int xEnd, int yEnd){
    int dx = fabs(x0 - xEnd), dy = fabs(y0 - yEnd);
    float yIncrementD, incrementXSY, m;
    int x = x0, y = y0;
    std::vector<BersenhamPointData> data;
    if(x0 > xEnd){
        x = xEnd;
        y = yEnd;
        xEnd = x0;
    }
    //斜率
    m = dx / dy;
    //2dy, 在pk < 0
    yIncrementD = 2 * dy;
    //2dy - 2dx, 在pk > 0;
    incrementXSY = yIncrementD - (2 * dx);
    //初始p0, 2dy - dx
    float p0 = yIncrementD - dx;
    data.push_back(BersenhamPointData(x, y, p0));
    //斜率绝对值下于1的情况下，是从x方向以单位步长开始移动的；如果斜率绝对大于1且斜率为正值，是从y方向以单位步长开始移动的;
    if(fabs(m) < 1){
        for(int i = 1; i < dx; ++i){
            if(data[i - 1].p < 0){
                float p = data[i - 1].p + yIncrementD;
                int x = data[i - 1].x + 1;
                int y = data[i - 1].y;
                data.push_back(BersenhamPointData(x, y, p));
            }else{
                float p = data[i - 1].p + incrementXSY;
                int x = data[i - 1].x + 1;
                int y = data[i - 1].y + 1;
                data.push_back(BersenhamPointData(x, y, p));
            }
        }
    }else{
        float xIncrementD = 2 * dx;
        incrementXSY = xIncrementD - yIncrementD;
        p0 = xIncrementD - dy;
        data.push_back(BersenhamPointData(x, y, p0));
        for(int i = 1; i < dy; ++i){
            if(data[i - 1].p < 0){
                float p = data[i - 1].p + xIncrementD;
                int x = data[i - 1].x;
                int y = data[i - 1].y + 1;
                data.push_back(BersenhamPointData(x, y, p));
            }else{
                float p = data[i - 1].p + incrementXSY;
                int x = data[i - 1].x + 1;
                int y = data[i - 1].y + 1;
                data.push_back(BersenhamPointData(x, y, p));
            }
        }
    }
    
    return data;
}
