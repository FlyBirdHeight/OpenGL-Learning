//
//  fluid.hpp
//  opengl
//
//  Created by adsionli on 2021/6/23.
//  Copyright © 2021 adsionli. All rights reserved.
//

#ifndef fluid_hpp
#define fluid_hpp

#include <stdio.h>
#include "point.hpp"
#include "grid.hpp"
#include "neigh.hpp"
#include "stdafx.hpp"
class FluidSystem{
    public:
        FluidSystem();
        void init(unsigned short maxPointCounts,
                  const glm::vec3& wallBox_min,const glm::vec3& wallBox_max,
                  const glm::vec3& initFluidBox_min,const glm::vec3& initFluidBox_max,
                  const glm::vec3& gravity){
            _init(maxPointCounts, fBox3(wallBox_min,wallBox_max),
                  fBox3(initFluidBox_min,initFluidBox_max), gravity);
        }
        //获取点的尺寸（字节）
        unsigned int getPointStride()const {return sizeof(Point);}
        //获取点的数量
        unsigned int getPointCounts()const{return m_pointBuffer.size();}
        //获取流体点缓存
        const Point* getPointBuf()const{return m_pointBuffer.get(0);}
        //逻辑桢
        void tick();
    //点位置缓存数据(x,y,z)
    std::vector<float> posData;
        ~FluidSystem();
    private:
        //初始化系统
        void _init(unsigned short maxPointCounts,const fBox3& wallBox,
                   const fBox3& initFluidBox, const glm::vec3& gravity);
        //计算密度，压力以及相邻关系
        void _computerPressure();
        //计算加速度
        void _computerForce();
        //移动粒子
        void _advance();
        //创建初始液体块
        void _addFluidVolume(const fBox3& fluidBox,float spacing);
        
        //数据成员
        PointBuffer m_pointBuffer;
        GridContainer m_gridContainer;
        NeighborTable m_neighborTable;
 

        
        //SPH光滑核
        float m_kernelPoly6;
        float m_kernelSpiky;
        float m_kernelViscosity;
        
        //其他参数
        float m_pointDistance;//半径
        float m_unitScale;//尺寸单位
        float m_viscosity;//粘性
        float m_restDensity;//静态密度
        float m_pointMass;//质量
        float m_smoothRadius;//光滑核半径
        float m_gasConstantK;//气体常量k
        float m_boundaryStiffness;//边界刚性
        float m_boundaryDampening;//边界阻尼
        float m_speedLimiting;//速度限制
        glm::vec3 m_gravityDir;//重力矢量
        
        int m_rexSize[3];//网格尺寸
        
        fBox3 m_sphWallBox;
    };

#endif /* fluid_hpp */
