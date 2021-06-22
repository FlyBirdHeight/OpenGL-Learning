//
//  grid_type.hpp
//  opengl
//
//  Created by adsionli on 2021/6/17.
//  Copyright © 2021 adsionli. All rights reserved.
//

#ifndef grid_type_hpp
#define grid_type_hpp

#include "sph_stdafx.hpp"
#include "point_type.hpp"
namespace SPH {
    /**
     * @description 三维下网格信息，最大最小赋值
     * @property {glm::fvec3} min 最小起始位置
     * @property {glm::fvec3} max 最大结束位置
    */
    class fBox3
    {
        public:
            fBox3() {}
            fBox3(const fBox3& other) : min(other.min), max(other.max) {}
            fBox3(const glm::fvec3& _min, const glm::fvec3& _max) : min(_min), max(_max) {}
            ~fBox3() {}

        public:
            glm::fvec3 min, max;
    };
    /**
     * @description 网格类，主要用来将空间划分成不同的网格，然后存储每一个网格内的粒子信息，方便之后的光滑核采样
     * @TODO 需要注意，这里的网格顺序是从左到右，在从下到上，最后才是从前到后，进行排序的
     *
     * @property {std::vector} m_gridData 网格信息，存储粒子下标
     * @property {glm::fvec3} m_gridMin 网格初始位置(左下角)
     * @property {glm::fvec3} m_gridMax 网格结束位置(右上角)
     * @property {glm::vec3} m_gridRes 网格规格(x, y, z方向上的粒子数 3*3*3||x*x*x)
     * @property {glm::fvec3} m_gridSize 网格尺寸(网格的总大小，x,y,z代表不同方向上的最大值)
     * @property {glm::fvec3} m_gridDelta 网格位置偏移（每一个网格在x,y,z上的偏移量）
     * @property {float} m_gridSpacing 网格各方向的偏移量
     */
    class GridType{
        public:
            GridType(){};
            void init(const fBox3 &box, float sim_scale, float cell_size, float border);
            void findCells(const glm::fvec3 position, float radius, int* gridCell);
            void insertParticles(PointType* points);
        
            int getGridData(int gridIndex);
            int findCell(const glm::fvec3 position);
            int getGridCellIndex(float dx, float dy, float dz, std::string type = "normal");
            
            ~GridType(){};
        private:
            std::vector<int> m_gridData;
            glm::fvec3 m_gridMin;
            glm::fvec3 m_gridMax;
            glm::ivec3 m_gridRes;
            glm::fvec3 m_gridSize;
            glm::fvec3 m_gridDelta;
            float m_gridSpacing;
        public:
            const glm::ivec3* getGridRes(void) const { return &m_gridRes; }
            const glm::fvec3* getGridMin(void) const { return &m_gridMin; }
            const glm::fvec3* getGridMax(void) const { return &m_gridMax; }
            const glm::fvec3* getGridSize(void) const { return &m_gridSize; }

    };
}

#endif /* grid_type_hpp */
