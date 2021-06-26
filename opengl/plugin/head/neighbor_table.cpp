//
//  neighbor_table.cpp
//  opengl
//
//  Created by adsionli on 2021/6/17.
//  Copyright © 2021 adsionli. All rights reserved.
//

#include "neighbor_table.hpp"

namespace SPH {
    NeighborTable::NeighborTable(): m_pointExtraData(0),
                                    pointCounts(0),
                                    pointCapcity(0),
                                    neighborDataBuf(0),
                                    dataBufSize(0),
                                    dataBufOffset(0),
                                    currPoint(0),
                                    currNeighborCounts(0) {};

    NeighborTable::~NeighborTable()
    {
        if(m_pointExtraData){
            delete [] m_pointExtraData;
            m_pointExtraData = nullptr;
        }
        if(neighborDataBuf){
            delete [] neighborDataBuf;
            neighborDataBuf=nullptr;
        }
    }
    /**
     * @brief 重置粒子邻接表
     * @param {short} pointsCount 构建邻接表粒子数量
     */
    void NeighborTable::reset(unsigned short pointsCount){
        int a = sizeof(PointExtraData);
        //@note 判断重置的粒子数量是否大于当前粒子容器的大小，如果大于粒子容器的大小，就重置粒子容器大小，并重置m_pointExtraData指向的内存空间大小
        if(pointsCount > this->pointCapcity){
            if(m_pointExtraData){
                delete [] m_pointExtraData;
                m_pointExtraData=nullptr;
            }
            m_pointExtraData = (PointExtraData*)malloc(a * pointsCount);
            this->pointCapcity = pointsCount;
        }
        this->pointCounts = pointsCount;
        memset(m_pointExtraData, 0, a * this->pointCapcity);
        this->dataBufOffset = 0;
    }
    /**
     * @brief 准备构建粒子邻接表准备
     * @param {short} pointIndex 当前粒子索引
     */
    void NeighborTable::point_prepare(unsigned short pointIndex){
        currPoint = pointIndex;
        currNeighborCounts = 0;
    }
    /**
     * @brief 向当前粒子中添加邻居粒子信息
     * @param {short} pointIndex 邻居粒子索引
     * @param {float} distance 邻居粒子到当前粒子的距离
     * @return true:添加成功 false:添加失败
     */
    bool NeighborTable::point_add_neighbor(unsigned short pointIndex, float distance){
        if(currNeighborCounts >= MAX_NEIGHTBOR_COUNTS){
            return false;
        }
        
        currNeighborIndex[currNeighborCounts] = pointIndex;
        currNeighborDistance[currNeighborCounts] = distance;
        currNeighborCounts++;
        
        return true;
    }

    /**
     * @brief 将当前粒子完成邻接表创建后提交到数据缓存中
     */
    void NeighborTable::point_commit(void){
        if(currNeighborCounts == 0){
            return;
        }
        /**
         * @note 获取邻接表索引所占的内存空间以及距离所占的内存空间大小，然后维护更新m_pointExtraData与dataBufSize
         */
        unsigned int index_size = currNeighborCounts * sizeof(unsigned short);
        unsigned int distance_size = currNeighborCounts * sizeof(float);
        if(dataBufOffset + index_size + distance_size > this->dataBufSize){
            this->_growDataBuf(dataBufOffset + index_size + distance_size);
        }
        this->m_pointExtraData[this->currPoint].neighborCounts = this->currNeighborCounts;
        this->m_pointExtraData[this->currPoint].neighborDataOffset = this->dataBufOffset;
        /**
         * @note 将数据加入字符缓存中去，同时维护dataBufOffset的大小
         */
        memcpy(this->neighborDataBuf + this->dataBufOffset, this->currNeighborIndex, index_size);
        this->dataBufOffset += index_size;
        memcpy(this->neighborDataBuf + this->dataBufOffset, this->currNeighborDistance, distance_size);
        this->dataBufOffset += distance_size;
    }
    /**
     * @brief 当数据提交时，所使用的数据空间大小大于设定数据空间大小时，增加设定数据空间大小
     * @note 主要思路：首先先获取新的空间大小，然后在对原有的缓存数据进行重新扩张。
     */
    void NeighborTable::_growDataBuf(unsigned int need_size){
        unsigned int newSize = this->dataBufSize > 0 ? dataBufSize : 1;
        while(newSize < need_size){
            newSize *= 2;
        }
        if (newSize < 1024) {
            newSize=1024;
        }
        unsigned char* newBuf = (unsigned char*)malloc(newSize);
        if(this->neighborDataBuf){
            memcpy(newBuf, this->neighborDataBuf, dataBufSize);
            delete [] this->neighborDataBuf;
        }
        
        this->neighborDataBuf = newBuf;
        dataBufSize = newSize;
    }

    /**
     * @brief 获取邻居信息
     * @param {short} pointIndex 需要获取的粒子邻接表的源粒子索引值
     * @param {int} index 邻居粒子在邻接表内的索引
     * @param {unsigned short &}neighborIndex 需要返回的邻居下标
     * @param {float &} neighborDistance 需要返回的邻居粒子与源粒子的距离
     */
    void NeighborTable::getNeighborInfo(unsigned short pointIndex, int index, unsigned short& neighborIndex, float& neighborDistance){
        PointExtraData pointData = this->m_pointExtraData[pointIndex];
        
        unsigned short* indexBuf = (unsigned short*)(this->neighborDataBuf + pointData.neighborDataOffset);
        float* distanceBuf = (float*)(this->neighborDataBuf + pointData.neighborDataOffset + sizeof(unsigned short) * pointData.neighborCounts);
        
        neighborIndex = indexBuf[index];
        neighborDistance = distanceBuf[index];
    }
}
