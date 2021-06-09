//
//  thread_bresenham.cpp
//  opengl
//
//  Created by adsionli on 2021/6/9.
//  Copyright © 2021 adsionli. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <vector>
#include <thread>

struct ThreadBersenhamPointData {
    int x;
    int y;
    float p;
    ThreadBersenhamPointData(int xN, int yN, float pN): x(xN), y(yN), p(pN){
        
    }
};
static std::vector<std::vector<ThreadBersenhamPointData>> returnData;
static int xS = 0, yS = 0, xE = 20, yE = 10;
static int dx = xE - xS, dy = yE - yS;

static int roundN(const float a){
    return int(a + 0.5);
}

static void bersenhamLine(int x0, int y0, int xEnd, int yEnd, int number, int dx_thread, int dy_thread){
    float yIncrementD, incrementXSY, m;
    int x = x0, y = y0;
    std::vector<ThreadBersenhamPointData> data;
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
    //初始p0, (k * dx_p) * 2dy - round(k * dy_p) * 2dx + 2dy - dx
    float p0 = number * dx_thread * yIncrementD - round(number * dy_thread) * (2 * dx) + yIncrementD - dx;
    data.push_back(ThreadBersenhamPointData(x, y, p0));
    int dxT = (xEnd - x);
    int dyT = (yEnd - y);
    //2dy, 在pk < 0
    yIncrementD = 2 * dyT;
    //2dy - 2dx, 在pk > 0;
    incrementXSY = yIncrementD - (2 * dxT);
    //斜率绝对值下于1的情况下，是从x方向以单位步长开始移动的；如果斜率绝对大于1且斜率为正值，是从y方向以单位步长开始移动的;
    if(fabs(m) < 1){
        for(int i = 1; i < dx; ++i){
            if(data[i - 1].p < 0){
                float p = data[i - 1].p + yIncrementD;
                int x = data[i - 1].x + 1;
                int y = data[i - 1].y;
                data.push_back(ThreadBersenhamPointData(x, y, p));
            }else{
                float p = data[i - 1].p + incrementXSY;
                int x = data[i - 1].x + 1;
                int y = data[i - 1].y + 1;
                data.push_back(ThreadBersenhamPointData(x, y, p));
            }
        }
    }else{
        float xIncrementD = 2 * dx;
        incrementXSY = xIncrementD - yIncrementD;
        p0 = xIncrementD - dy;
        data.push_back(ThreadBersenhamPointData(x, y, p0));
        for(int i = 1; i < dy; ++i){
            if(data[i - 1].p < 0){
                float p = data[i - 1].p + xIncrementD;
                int x = data[i - 1].x;
                int y = data[i - 1].y + 1;
                data.push_back(ThreadBersenhamPointData(x, y, p));
            }else{
                float p = data[i - 1].p + incrementXSY;
                int x = data[i - 1].x + 1;
                int y = data[i - 1].y + 1;
                data.push_back(ThreadBersenhamPointData(x, y, p));
            }
        }
    }
    
    returnData.push_back(data);
}

static std::vector<std::vector<ThreadBersenhamPointData>> createThreadBersenhamLine(){
    
    std::vector<std::thread> threadPool;
    int maxThreadCount = std::thread::hardware_concurrency();
    float k = dy / dx;
    int threadCount = maxThreadCount - 2;
    int dx_thread = floor(double((dx + threadCount - 1) / threadCount));
    int dy_thread = floor(double(k * dx_thread));
    int x = xS, y = yS;
    for(int i = 0; i < threadCount; ++i){
        int xET = xS + (i + 1) * dx_thread;
        int yET = yS + roundN((i + 1) * dy_thread);
        std::thread th(bersenhamLine, x, y, xET, yET, i, dx_thread, dy_thread);
        x = xET;
        y = yET;
        threadPool.push_back(th);
    }
    for(int i = 0; i < threadPool.size(); ++i){
        if(threadPool[i].joinable()){
            threadPool[i].join();
        }
    }
    
    return returnData;
}
