//
//  point_level.hpp
//  opengl
//  @brief 离散点的水平集构建类
//  Created by adsionli on 2021/6/26.
//  Copyright © 2021 adsionli. All rights reserved.
//

#ifndef point_level_hpp
#define point_level_hpp

#include "sph_stdafx.hpp"
#include "fluid_system.hpp"
namespace SPH {
    class PointLevel {
        public:
            PointLevel(FluidSystem* fluidSystem);
            struct GridPointLevel {
                float distance;
                int index;
                GridPointLevel():distance(FLT_MAX), index(-1){};
            };
            GridPointLevel* getGridPointLevel(){
                return m_gridPointLevel;
            }
            /**
             * @brief 构建水平集
             */
            void generatePointLevel();
            ~PointLevel();
        private:
            GridPointLevel* m_gridPointLevel;
            FluidSystem* m_fluidSystem;
            GridType* m_gridType;
            PointType* m_pointType;
            /**
             * @brief 当前欧拉网格点扩散到其他欧拉网格点
             * @property {GridPointLevel*} gridPointLevel 当前网格点的地址
             */
            void spreadNeighbor(GridPointLevel* gridPointLevel);
    };
};

#endif /* point_level_hpp */
