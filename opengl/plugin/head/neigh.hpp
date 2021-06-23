//
//  neigh.hpp
//  opengl
//
//  Created by adsionli on 2021/6/23.
//  Copyright © 2021 adsionli. All rights reserved.
//

#ifndef neigh_hpp
#define neigh_hpp

#include <stdio.h>
#include "stdafx.hpp"
#include "point.hpp"
#include "grid.hpp"
#define MAX_NEIGHBOR_COUNTS 80
    class NeighborTable{
    public:
        NeighborTable();
        void reset(unsigned short pointCounts);//重置邻接表
        void point_prepare(unsigned short ptIndex);//预备点数据
        bool point_add_neighbor(unsigned short ptIndex,float distance);//给当前点添加邻接表
        void point_commit();//递交点给邻接表
        int getNeighborCounts(unsigned short ptIndex){return m_pointExtraData[ptIndex].neighborCounts;}//获取邻接表中的点个数
        void getNeighborInfo(unsigned short ptIndex,int index, unsigned short& neighborIndex, float& neighborDistance);//获取索引ptIndex的邻接表中第index个点的数据
        
        ~NeighborTable();
    private:
        union PointExtraData{
            struct{
                unsigned neighborDataOffset:24;//偏移
                unsigned neighborCounts:8;//个数
            };
        };
        PointExtraData* m_pointExtraData;//邻接表信息
        unsigned int m_pointCounts;//粒子数
        unsigned int m_pointCapacity;//粒子容量
        unsigned char* m_neighborDataBuf;// 邻接表的数据缓存
        unsigned int m_dataBufSize;//bytes 数据缓存尺寸
        unsigned int m_dataBufOffset;//bytes 数据缓存中的偏移
        
        //当前点点数据
        unsigned short m_currPoint;//索引
        int m_currNeighborCounts;//邻居点数量
        unsigned short m_currNeightborIndex[MAX_NEIGHBOR_COUNTS];//邻居中点的索引
        float m_currNrighborDistance[MAX_NEIGHBOR_COUNTS];//邻居中点的距离
        
        void _growDataBuf(unsigned int need_size);//扩容
    };

#endif /* neigh_hpp */
