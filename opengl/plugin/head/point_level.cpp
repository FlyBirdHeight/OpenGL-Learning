//
//  point_level.cpp
//  opengl
//
//  Created by adsionli on 2021/6/26.
//  Copyright © 2021 adsionli. All rights reserved.
//

#include "point_level.hpp"

namespace SPH {
    PointLevel::PointLevel(FluidSystem* fluidSystem){
        m_fluidSystem = fluidSystem;
        m_pointType = m_fluidSystem->getPointType();
        m_gridType = m_fluidSystem->getGridType();
    }

    /**
     * @brief 构建水平集
     */
    void PointLevel::generatePointLevel(){
        m_gridPointLevel = (GridPointLevel*)malloc(m_gridType->getGridCounts() * (int)sizeof(GridPointLevel));
        unsigned int pointSize = m_fluidSystem->getPointCounts();
        Point* point = m_pointType->get(0);
        for(int i = 0; i < pointSize; i++){
            
        }
    }

    void PointLevel::spreadNeighbor(GridPointLevel *gridPointLevel){
        for(int x = -1; x < 2; ++x){
            for(int y = -1; y < 2; ++y){
                for(int z = -1; z < 2; ++z){
                    if(x == 0 && y == 0 && z == 0){
                        continue;
                    }
                }
            }
        }
    }
}
