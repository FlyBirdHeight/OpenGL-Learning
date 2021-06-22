//
//  neighbor_table.hpp
//  opengl
//
//  Created by adsionli on 2021/6/17.
//  Copyright © 2021 adsionli. All rights reserved.
//

#ifndef neighbor_table_hpp
#define neighbor_table_hpp

#include "sph_stdafx.hpp"
#include <iostream>
namespace SPH{
    struct PointExtraData{
        unsigned int neighborDataOffset = 24;
        unsigned int neighborCounts = 8;
    };
    /**
     * @brief 邻接表构建类
     * @note 本类中的数据信息缓存全部是使用进制形式进行存储的，其实也可以使用数组、队列等方法进行存储，也就是neighborBufData这个变量，可以选用自己喜欢的形式。
     */
    class NeighborTable{
    public:
        NeighborTable();
        /**
         * @brief 重置粒子邻接表
         * @param {short} pointsCount 构建邻接表粒子数量
         */
        void reset(unsigned short pointsCount);
        /**
         * @brief 准备构建粒子邻接表准备
         * @param {short} pointIndex 当前粒子索引
         */
        void point_prepare(unsigned short pointIndex);
        /**
         * @brief 向当前粒子中添加邻居粒子信息
         * @param {short} pointIndex 邻居粒子索引
         * @param {float} distance 邻居粒子到当前粒子的距离
         * @return true:添加成功 false:添加失败
         */
        bool point_add_neighbor(unsigned short pointIndex, float distance);
        /**
         * @brief 将当前粒子完成邻接表创建后提交到数据缓存中
         */
        void point_commit(void);
        /**
         * @brief 获取邻居信息
         * @param {short} pointIndex 需要获取的粒子邻接表的源粒子索引值
         * @param {int} index 邻居粒子在邻接表内的索引
         * @param {unsigned short &}neighborIndex 需要返回的邻居下标
         * @param {float &} neighborDistance 需要返回的邻居粒子与源粒子的距离
         */
        void getNeighborInfo(unsigned short pointIndex, int index, unsigned short& neighborIndex, float& neighborDistance);
        /**
         * @brief 获取指定粒子邻居的数量
         */
        int getNeighborCounts(unsigned int pointIndex){
            return (m_pointExtraData+pointIndex)->neighborCounts;
        }
        ~NeighborTable();
    private:
        void _growDataBuf(unsigned int need_size);
        /**
         * @property {struct} PointExtraData 粒子领接表，记录粒子邻居数据数量及偏移值
         * @property {PointExtraData} m_pointExtraData 领接表数据集合，记录每一个粒子的领接表
         * @property {int} pointCounts 粒子数量
         * @property {int} pointCapcity 粒子容器的大小
         * @property {char*} neighborDataBuf 邻接表数据缓存对象 bytes
         * @property {int} dataBufSize 数据缓存尺寸 bytes
         * @property {int} dataBufOffset 数据缓存偏移量 bytes
         * @property {short} currPoint 当前粒子的数据索引值
         * @property {int} currNeighborCount 当前粒子的邻居数量
         * @property {short[]} currNeighborIndex 当前粒子的邻居索引集合
         * @property {float[]} currNeighborDistance 当前粒子对应的邻居到当前粒子距离的集合
         */
        
        PointExtraData* m_pointExtraData;
        unsigned int pointCounts;
        unsigned int pointCapcity;
        unsigned char* neighborDataBuf;
        unsigned int dataBufSize;
        unsigned int dataBufOffset;
        
        unsigned short currPoint;
        int currNeighborCounts;
        unsigned short currNeighborIndex[MAX_NEIGHTBOR_COUNTS];
        float currNeighborDistance[MAX_NEIGHTBOR_COUNTS];
    };
};

#endif /* neighbor_table_hpp */
