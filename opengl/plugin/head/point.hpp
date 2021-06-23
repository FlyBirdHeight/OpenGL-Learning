//
//  point.hpp
//  opengl
//
//  Created by adsionli on 2021/6/23.
//  Copyright © 2021 adsionli. All rights reserved.
//

#ifndef point_hpp
#define point_hpp

#include "stdafx.hpp"
struct Point{
        glm::vec3 pos;//点位置
        float density;//密度
        float pressure;//压力
        glm::vec3 accel;//加速度
        glm::vec3 velocity;//速度
        glm::vec3 velocity_eval;//最后速度
        
        int next;//指向下一个点的索引
    };
    
    class PointBuffer{
    public:
        PointBuffer();
        void reset(unsigned int capacity);//重置粒子点缓冲
        unsigned int size()const{return m_fluidCounts;}//返回粒子个数
        
        //获取索引为index的点
        Point* get(unsigned int index){return m_FluidBuf+index;}
        const Point* get(unsigned int index) const {return m_FluidBuf+index;}
        
        Point* addPointReuse();//缓冲中加入新的点并返回
        virtual ~PointBuffer();
        
    private:
        Point* m_FluidBuf;//粒子点缓存
        unsigned int m_fluidCounts;//粒子个数
        unsigned int m_BufCapcity;//粒子容量
};

#endif /* point_hpp */
