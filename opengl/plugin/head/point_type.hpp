//
//  point_type.hpp
//  opengl
//
//  Created by adsionli on 2021/6/16.
//  Copyright © 2021 adsionli. All rights reserved.
//

#ifndef point_type_hpp
#define point_type_hpp

#include "sph_stdafx.hpp"
namespace SPH
{
    struct Point{
        public:
            //粒子位置
            glm::fvec3 position;
            //粒子密度
            float density;
            //粒子压强
            float pressure;
            //粒子加速度
            glm::fvec3 accel;
            //粒子速度
            glm::fvec3 velocity;
            //粒子速度评估
            glm::fvec3 velocity_eval;
            //粒子下一个位置下标
            int next;
    };
    
class PointType{
public:
    PointType();
    //重置流体粒子数量，并重构
    void reset(unsigned int capcity);
    //获取流体粒子数量大小
    unsigned int size(void) const {
        return pointSize;
    };
    //获取对应位置下标的流体粒子参数(可修改)
    Point* get(unsigned int index){
        return points[index];
    };
    //获取对应位置下标的流体粒子参数(不可修改)
    const Point* get(unsigned int index) const {
        return points[index];
    };
    //添加新的流体粒子
    Point* addPointReuse(void);
private:
    //流体粒子数量
    unsigned int pointSize;
    //容器容积
    unsigned int capcity;
    //流体粒子保存的队列
    std::vector<Point*> points;
    //枚举参数，设置最大容器粒子数
    enum {
        ELEM_MAX = 4096
    };
};
}

#endif /* point_type_hpp */
