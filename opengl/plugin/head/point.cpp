//
//  point.cpp
//  opengl
//
//  Created by adsionli on 2021/6/23.
//  Copyright © 2021 adsionli. All rights reserved.
//

#include "point.hpp"
PointBuffer::PointBuffer():m_FluidBuf(0),m_fluidCounts(0),m_BufCapcity(0){}
    PointBuffer::~PointBuffer(){
        delete[] m_FluidBuf;
        m_FluidBuf=nullptr;
    }
 
    void PointBuffer::reset(unsigned int capacity){
        m_BufCapcity=capacity;
        if(m_FluidBuf!=0){//当点缓存不为空，则清空点缓存
            delete [] m_FluidBuf;
            m_FluidBuf=nullptr;
        }
        if(m_BufCapcity>0)//给点缓存分配空间
            m_FluidBuf=new Point[m_BufCapcity]();
        m_fluidCounts=0;//设置点数量为0
    }
 
    Point* PointBuffer::addPointReuse(){
        if(m_fluidCounts>=m_BufCapcity){
            if(m_BufCapcity*2>(4096*2)){
                //当超过上限时，返回最后值
                return m_FluidBuf+m_fluidCounts-1;
            }
            m_BufCapcity*=2;
            Point* new_data=new Point[m_BufCapcity]();
            memcpy(new_data, m_FluidBuf, m_fluidCounts*sizeof(Point));
            delete [] m_FluidBuf;
            m_FluidBuf=new_data;
        }
        //新的点
        Point* point=m_FluidBuf+m_fluidCounts++;
        point->pos=glm::vec3(0);
        point->next=0;
        point->velocity=glm::vec3(0);
        point->velocity_eval=glm::vec3(0);
        point->pressure=0;
        point->density=0;
        point->accel=glm::vec3(0);
 
        return point;

    }
