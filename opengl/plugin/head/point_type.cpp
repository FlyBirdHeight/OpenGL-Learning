//
//  point_type.cpp
//  opengl
//
//  Created by adsionli on 2021/6/16.
//  Copyright © 2021 adsionli. All rights reserved.
//

#include "point_type.hpp"
namespace SPH{
    PointType::PointType(): m_pointSize(0), m_capcity(0){
        
    }
    
    PointType::~PointType(){
        delete[] points;
        points = nullptr;
    }

    void PointType::reset(unsigned int capcity){
        m_capcity = capcity;
        if(points != 0){
            free(points);
            points = 0;
        }
        //创建初值指针地址对象
        if(m_capcity > 0){
            points = (Point*)malloc(m_capcity * sizeof(Point));
        }
        m_pointSize = 0;
    }

    Point* PointType::addPointReuse(void){
        /*
         * 判断当前粒子数量是否大于等于容器容积，如果大于就将容器大小扩大2倍，
         * 如果扩大后容器大小超过界限，就返回在范围内的随机一个值，
         * 如果容器扩大二倍后，未超过界限，就重新创建一个新的对象并赋值其地址为结构体地址，然后将
         * 原容器中数据复制过去，在赋值给源数据即可。
         */
        if(m_pointSize >= m_capcity){
            if(m_capcity * 2 > ELEM_MAX){
                return points + m_pointSize - 1;
            }
            m_capcity *= 2;
            Point* new_data = (Point*)malloc(m_capcity * sizeof(Point));
            memcpy(new_data, points, m_pointSize * sizeof(Point));
            free(this->points);
            this->points = new_data;
        }
        
        Point* point = points + (m_pointSize++);
        point->position = glm::fvec3(0, 0, 0);
        point->next = 0;
        point->velocity = glm::fvec3(0,0,0);
        point->velocity_eval = glm::fvec3(0,0,0);
        point->pressure = 0;
        point->density = 0;
        point->accel = glm::fvec3(0,0,0);
        
        return point;
    }
}
