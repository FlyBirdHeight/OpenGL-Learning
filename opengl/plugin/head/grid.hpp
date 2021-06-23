//
//  grid.hpp
//  opengl
//
//  Created by adsionli on 2021/6/23.
//  Copyright © 2021 adsionli. All rights reserved.
//

#ifndef grid_hpp
#define grid_hpp

#include "stdafx.hpp"
#include "point.hpp"
class fBox3{
public:
    fBox3():min(glm::vec3(0)),max(glm::vec3(0)){}
    fBox3(glm::vec3 aMin,glm::vec3 aMax):min(aMin),max(aMax){}
    glm::vec3 min,max;
};

class GridContainer{
public:
    GridContainer(){}
    ~GridContainer(){}
    //空间细分
    void init(const fBox3& box,float sim_scale,float cell_size,float border,int* rex);
    void insertParticles(PointBuffer*pointBuffer);
    void findCells(const glm::vec3& p,float radius,int *gridCell);
    void findTwoCells(const glm::vec3& p,float radius,int *gridCell);
    int findCell(const glm::vec3& p);
    int getGridData(int gridIndex);
    
    const glm::ivec3* getGridRes()const{return &m_GridRes;}
    const glm::vec3* getGridMin(void) const { return &m_GridMin; }
    const glm::vec3* getGridMax(void) const { return &m_GridMax; }
    const glm::vec3* getGridSize(void) const { return &m_GridSize; }
    
    int getGridCellIndex(float px,float py,float pz);//获取对应xyz下的网格索引
    float getDelta(){return m_GridDelta.x;}
    
private:
    //空间网格
    std::vector<int> m_gridData;//表格信息（储存网格内的当前的粒子）
    glm::vec3 m_GridMin;//表格左下角
    glm::vec3 m_GridMax;//表格右上角
    glm::ivec3 m_GridRes;//表格规格（n * m * l）
    glm::vec3 m_GridSize;//表格c大小
    glm::vec3 m_GridDelta;//表格偏移量
    float m_GridCellSize;//一个格子大小（通常为2倍的光滑核半径）
};
#endif /* grid_hpp */
