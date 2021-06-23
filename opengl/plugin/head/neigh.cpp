//
//  neigh.cpp
//  opengl
//
//  Created by adsionli on 2021/6/23.
//  Copyright © 2021 adsionli. All rights reserved.
//

#include "neigh.hpp"
NeighborTable::NeighborTable():
    m_pointExtraData(0),
    m_pointCounts(0),
    m_pointCapacity(0),
    m_neighborDataBuf(0),
    m_dataBufSize(0),
    m_dataBufOffset(0),
    m_currPoint(0),
    m_currNeighborCounts(0){
    }
 
    NeighborTable::~NeighborTable(){
        if(m_pointExtraData){
            delete [] m_pointExtraData;
            m_pointExtraData=nullptr;
        }
        if(m_neighborDataBuf){
            delete [] m_neighborDataBuf;
            m_neighborDataBuf=nullptr;
        }
    }
 
    void NeighborTable::reset(unsigned short pointCounts){
        int a=sizeof(PointExtraData);
        if(pointCounts>m_pointCapacity){
            if(m_pointExtraData){
                delete [] m_pointExtraData;
                m_pointExtraData=nullptr;
            }
            m_pointExtraData=new PointExtraData[a*pointCounts]();
            m_pointCapacity=m_pointCounts;
        }
        m_pointCounts=pointCounts;
        memset(m_pointExtraData, 0, a*m_pointCapacity);
        m_dataBufOffset=0;
    }
    //准备点
    void NeighborTable::point_prepare(unsigned short ptIndex){
        m_currPoint=ptIndex;
        m_currNeighborCounts=0;
    }
 
    bool NeighborTable::point_add_neighbor(unsigned short ptIndex, float distance){
        if(m_currNeighborCounts>=MAX_NEIGHBOR_COUNTS)return false;
        m_currNeightborIndex[m_currNeighborCounts]=ptIndex;
        m_currNrighborDistance[m_currNeighborCounts]=distance;
        m_currNeighborCounts++;
        return true;
    }
    void NeighborTable::point_commit(){
        if(m_currNeighborCounts==0)return;
        unsigned int index_size=m_currNeighborCounts*sizeof(unsigned short);
        unsigned int distance_size=m_currNeighborCounts*sizeof(float);
 
        //扩大空间
        if(m_dataBufOffset+index_size+distance_size>m_dataBufSize)
            _growDataBuf(m_dataBufOffset+index_size+distance_size);
 
        //设置邻居数据
        m_pointExtraData[m_currPoint].neighborCounts=m_currNeighborCounts;
        m_pointExtraData[m_currPoint].neighborDataOffset=m_dataBufOffset;
 
        //复制索引点的信息到数据缓存中
        memcpy(m_neighborDataBuf+m_dataBufOffset, m_currNeightborIndex, index_size);
        m_dataBufOffset+=index_size;
        memcpy(m_neighborDataBuf+m_dataBufOffset, m_currNrighborDistance, distance_size);
        m_dataBufOffset+=distance_size;
    }
 
    void NeighborTable::_growDataBuf(unsigned int need_size){
        unsigned int newSize=m_dataBufSize>0?m_dataBufSize:1;
        while (newSize<need_size) {
            newSize*=2;
        }
        if (newSize<2024) {
            newSize=1024;
        }
        unsigned char* newBuf=new unsigned char[newSize]();
        if(m_neighborDataBuf){
            memcpy(newBuf, m_neighborDataBuf, m_dataBufSize);
            delete [] m_neighborDataBuf;
        }
        m_neighborDataBuf=newBuf;
        m_dataBufSize=newSize;
    }
 
    //ptIndex是点的索引，index是该点邻居表内的邻居索引,获取的neighborIndex为该邻居的默认索引
    void NeighborTable::getNeighborInfo(unsigned short ptIndex, int index, unsigned short &neighborIndex, float &neighborDistance){
        PointExtraData neighData=m_pointExtraData[ptIndex];
        unsigned short* indexBuf=(unsigned short*)(m_neighborDataBuf+neighData.neighborDataOffset);
        float* distanceBuf=(float*)(m_neighborDataBuf+neighData.neighborDataOffset+sizeof(unsigned short)*neighData.neighborCounts);
 
        neighborIndex=indexBuf[index];
        neighborDistance=distanceBuf[index];
    }


