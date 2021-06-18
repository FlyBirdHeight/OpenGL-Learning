//
//  fluid_system.hpp
//  opengl
//
//  Created by adsionli on 2021/6/18.
//  Copyright © 2021 adsionli. All rights reserved.
//

#ifndef fluid_system_hpp
#define fluid_system_hpp

#include "fluid_interface.h"
#include "sph_stdafx.hpp"
#include "point_type.hpp"
#include "neighbor_table.hpp"
#include "grid_type.hpp"

namespace SPH {
    class FluidSystem : public System{
        public:
            /**
             * @brief 初始化流体系统，设置流体范围
             * @param {short} maxPointCounts 最大流体粒子数量
             * @param {glm::fvec3*} wallBox_min 墙体盒子最小网格位置
             * @param {glm::fvec3*} wallBox_max 墙体盒子最大网格位置
             * @param {glm::fvec3*} initFluidBox_min 流体盒子最小网格位置
             * @param {glm::fvec3*} initFluidBox_max 流体盒子最大网格位置
             * @param {glm::fvec3*} gravity 重力加速度
             */
            virtual void init(unsigned short maxPointCounts,
                      const glm::fvec3* wallBox_min, const glm::fvec3* wallBox_max,
                      const glm::fvec3* initFluidBox_min, const glm::fvec3* initFluidBox_max,
                      const glm::fvec3* gravity)
            {
                fBox3* wallBox = new fBox3(*wallBox_min, *wallBox_max);
                fBox3* fluidBox = new fBox3(*initFluidBox_min, *initFluidBox_max);
                this->_init(maxPointCounts, *wallBox, *fluidBox, *gravity);
            }
            /**
             * @brief 获取点结构体的内存空间大小
             */
            virtual unsigned int getPointStride(void) const {
                return sizeof(Point);
            }
            /**
             * @brief 获取流体粒子数量
             * @return 流体粒子数量
             */
            virtual unsigned int getPointCounts(void) const {
                return m_pointType.size();
            }
            /**
             * @brief 获取流体粒子信息
             * @return 粒子信息结构体
             */
            virtual const Point* getPointBuf(void) const {
                return m_pointType.get(0);
            }
            /**
             * @brief 逻辑帧
             */
            virtual void tick(void);
        private:
            /**
             * @brief 初始化流体系统，设置流体范围
             * @param {short} maxPointCounts 最大流体粒子数量
             * @param {fBox3} wallBox 墙体盒子网格范围
             * @param {fBox3} initFluidBox_min 流体盒子网格范围
             * @param {glm::fvec3*} gravity 重力加速度
             */
            void _init(unsigned short maxPointCounts, const fBox3& wallBox, const fBox3& initFluidBox, const glm::fvec3& gravity);
            /**
             * @brief 计算粒子所受压强及粒子密度
             */
            void computePressure(void);
            /**
             * @brief 计算粒子加速度
             */
            void computeForce(void);
            /**
             * @brief 粒子移动
             */
            void advance(void);
            /**
             * @brief 创建初始液体块
             * @param {fBox3} fluidBox 流体盒子
             * @param {float} spacing 网格间隔
             */
            void addFluidVolume(const fBox3& fluidBox, float spacing);
            PointType m_pointType;
            GridType m_gridType;
            NeighborTable m_neighborTable;
            /**
             * @description SPH光滑核模型
             * @property {float} kernelPoly6 就算密度时使用的光滑核函数
             * @property {float} kernelSpiky 计算压强时使用的光滑核函数
             * @property {float} kernelViscosity 计算粘度(相互作用力)使用的光滑核函数
             */
            float m_kernelPoly6;
            float m_kernelSpiky;
            float m_kernelViscosity;
            /**
             * @description 一部分限制参数或设定参数
             * @property {float} m_unitScale 实际空间缩放比例[构建网格时传入]
             * @property {float} m_viscosity 空间中粒子的初始速度
             * @property {float} m_restDensity 粒子不运动时所具有的密度
             * @property {float} m_pointMass 粒子的质量
             * @property {float} m_smoothRadius 光滑核半径
             * @property {float} m_gasConstantK 大气密度
             * @property {float} m_boundartStiffness 边界刚度(起到的作用是限制粒子在边界时进行反弹而不至于弹出且边界不会移动)
             * @property {float} m_boundaryDampening 边界阻尼(用于控制粒子运动过程趋于平缓且周期性)
             * @property {float} m_speedLimiting 速度限制
             * @property {glm::fvec3} m_gravityDir 重力加速度
             * @property {float} m_pointDistance 粒子半径
             */
            float m_unitScale;
            float m_viscosity;
            float m_restDensity;
            float m_pointMass;
            float m_smoothRadius;
            float m_gasConstantK;
            float m_boundartStiffness;
            float m_boundaryDampening;
            float m_speedLimiting;
            float m_pointDistance;
            glm::fvec3 m_gravityDir;
            int m_rexSize[3];
            fBox3 m_sphWallBox;
        };
}

#endif /* fluid_system_hpp */
