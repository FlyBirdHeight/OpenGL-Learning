//
//  grid_type.cpp
//  opengl
//
//  Created by adsionli on 2021/6/17.
//  Copyright © 2021 adsionli. All rights reserved.
//

#include "grid_type.hpp"
#include <iostream>
namespace SPH{
    enum {
        POINT_START = 0
    };
    /**
     * @brief 初始化网格体系
     * @param {fBox3} box 三维下初始位置，结束位置记录类
     * @param {float} sim_scale 缩放大小
     * @param {float} cell_size 单个网格大小
     * @param {float} border 网格边界
     */
    void GridType::init(const fBox3 &box, float sim_scale, float cell_size, float border){
        //实际网格大小
        float real_gridSize = cell_size / sim_scale;
        //计算边界值，需要考虑border(边界)位置
        m_gridMin = box.min;
        m_gridMax = box.max;
        m_gridMin += border;
        m_gridMax -= border;
        //这里只是初始化一下网格在不取整时候，网格的最大与最小之间间隔（这里不是最终的尺寸，只是物理意义上的大小）
        m_gridSize = m_gridMax - m_gridMin;
        //网格间隔
        m_gridSpacing = real_gridSize;
        //初始化网格规格
        m_gridRes = glm::ivec3(std::ceil(m_gridSize.x / real_gridSize),
                               std::ceil(m_gridSize.y / real_gridSize),
                               std::ceil(m_gridSize.z / real_gridSize));
        //调整网格尺寸为网格规格的整数倍，以容纳足够的网格
        m_gridSize = glm::fvec3(m_gridRes.x * real_gridSize,
                                m_gridRes.y * real_gridSize,
                                m_gridRes.z * real_gridSize);
        //这个偏移是基于屏幕空间的实际偏移量，每一个网格的
        m_gridDelta = m_gridRes;
        m_gridDelta /= m_gridSize;

        int gridTotal = (int)(m_gridRes.x * m_gridRes.y * m_gridRes.z);
        m_gridData.resize(gridTotal);
    }

    /**
     * @brief 获取指定下标网格位置
     * @param {int} gridIndex 指定网格下标
     * @returns {int} 邻接表位置下标
     */
    int GridType::getGridData(int gridIndex){
        if(gridIndex < 0 || gridIndex > m_gridData.size()){
            return -1;
        }
        
        return m_gridData[gridIndex];
    }

    /**
     * @brief 在网格中添加粒子
     * @param {PointType*} points 指定添加粒子
     * @note 实现思路：
     *  1. 在m_gridData实际是一张hashmap，且其中每一项存储的是一个粒子队列的头指针（这个队列生成时是倒插的）。
     *  2. 所以首先需要将其全部初始化为-1，因为此时每个网格内的粒子队列为空，所以其头指针是不存在的。
     *  3. 然后开始遍历粒子数据，并在每一次遍历时获取粒子所在网格空间的下标。
     *  4 .如果不在网格空间内，那就意味着这个粒子不在我们要显示的范围内，所以其粒子队列就不存在。
     *  5. 如果在网格空间中，就开始建立粒子队列，如果是当前网格空间中的第一个粒子，
     *    那么其next指针指向的下一位为-1(因为队列的生成是反过来的，所以第一位进去的就是当前网格空间中的最后一位)
     *  6. 之后再对hashmap中对应的网格空间更新其头指针的位置，方便下一次在当前网格空间的粒子插入粒子队列中
     */
    void GridType::insertParticles(PointType* points){
        //批量初试化变量
        std::fill(m_gridData.begin(), m_gridData.end(), -1);
        Point* p = points->get(POINT_START);
        for(unsigned int i = 0; i < points->size(); ++i, ++p){
            int gridPosition = this->getGridCellIndex(p->position.x, p->position.y, p->position.z, "normal");
            if(gridPosition >= 0 && gridPosition < (int)m_gridData.size()){
                p->next = m_gridData[gridPosition];
                m_gridData[gridPosition] = i;
            }else{
                p->next = -1;
            }
        }
    }

    /**
     * @brief 寻找本网格内的粒子信息
     * @param {glm::fvec3} position 粒子位置
     */
    int GridType::findCell(const glm::fvec3 position){
        int gridPosition = getGridCellIndex(position.x, position.y, position.z, "normal");
        if(gridPosition < 0 || gridPosition >= m_gridData.size()){
            return -1;
        }
        
        return gridPosition;
    }

    /**
     * @brief 寻找领域网格内的粒子信息
     * @param {glm::fvec3} position 粒子位置
     * @param {float} radius 光滑核半径
     * @param {int*} gridCell 返回结果
     */
    void GridType::findCells(const glm::fvec3 position, float radius, int* gridCell){
        for(int i = 0; i < 8; i++){
            gridCell[i] = -1;
        }
        //获取当前粒子形成的光滑核最小位置坐标，用来将相交的网格空间定位在其左下角(因为三维空间，所以实际为左下角靠后的那个)
        int minX = (int)((-radius + position.x - m_gridMin.x) * m_gridDelta.x);
        int minY = (int)((-radius + position.y - m_gridMin.y) * m_gridDelta.y);
        int minZ = (int)((-radius + position.z - m_gridMin.z) * m_gridDelta.z);
        if(minX < 0){
            minX = 0;
        }
        if(minY < 0){
            minY = 0;
        }
        if(minZ < 0){
            minZ = 0;
        }
        //下层的相交的网格空间
        gridCell[0] = (minZ * m_gridRes.y + minZ) * m_gridRes.x + minX;
        gridCell[1] = gridCell[0] + 1;
        gridCell[2] = gridCell[0] + m_gridRes.x;
        gridCell[3] = gridCell[2] + 1;
        //上层相交的网格空间
        if((minZ + 1) < m_gridRes.z){
            gridCell[4] = gridCell[0] + m_gridRes.x * m_gridRes.y;
            gridCell[5] = gridCell[4] + 1;
            gridCell[6] = gridCell[4] + m_gridRes.x;
            gridCell[7] = gridCell[6] + 1;
        }
        //当网格空间是最靠右边的时候(x = m_gridMax.x)，需要将另外的半边(右侧一面四个网格即 1, 3, 5, 7)裁切掉，因为已经超出网格空间了
        if((minX + 1) >= m_gridRes.x){
            gridCell[1] = -1;
            gridCell[3] = -1;
            gridCell[5] = -1;
            gridCell[7] = -1;
        }
        //当网格空间是最靠上边的时候(y = m_gridMax.y)，需要将另外的半边(前侧一面四个网格即 2, 3, 6, 7)裁切掉，因为已经超出网格空间了
        if((minY + 1) >= m_gridRes.y){
            gridCell[2] = -1;
            gridCell[3] = -1;
            gridCell[6] = -1;
            gridCell[7] = -1;
        }
        
    }

    /**
     * @description 获取网格数据中位置的下标
     * @param {float} x位置坐标
     * @param {float} y位置坐标
     * @param {float} z位置坐标
     */
    int GridType::getGridCellIndex(float dx, float dy, float dz, std::string type){
        if(type == "normal"){
            //@note 这里对数据的处理是为了让其落在对应的网格中去
            int gx = (int)((dx - m_gridMin.x) * m_gridDelta.x);
            int gy = (int)((dy - m_gridMin.y) * m_gridDelta.y);
            int gz = (int)((dz - m_gridMin.z) * m_gridDelta.z);
            //@note 这里实际就是按照自己规定的方向去数格子即可
            return gx + gz * m_gridRes.x * m_gridRes.y + gy * m_gridRes.x;
        }else{
            return (dz * m_gridRes.y + dy) * m_gridRes.x + dx;
        }
        
    }
}
