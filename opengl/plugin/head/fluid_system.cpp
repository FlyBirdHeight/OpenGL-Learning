//
//  fluid_system.cpp
//  opengl
//
//  Created by adsionli on 2021/6/18.
//  Copyright © 2021 adsionli. All rights reserved.
//

#include "fluid_system.hpp"
#define PI 3.1415926

namespace SPH {
    /**
     * @brief 初始化参数
     * 比例，粘度，流体密度，粒子重量，大气密度，光滑核半径，刚体边界，边界阻尼，速度限制
     * 光滑和函数Poly6(计算粒子密度)，光滑核函数Spiky(计算粒子压强)，光滑和函数Viscosity(计算粒子粘性力，实际就是所受的切应力)
     */
    FluidSystem::FluidSystem(){
        m_unitScale = 0.004f;
        m_viscosity = 1.0f;
        m_restDensity = 1000.0f;
        m_pointMass = 0.0004f;
        m_gasConstantK = 1.0f;
        m_smoothRadius = 0.01f;
        m_realRadius = m_smoothRadius / m_unitScale;
        
        m_boundartStiffness = 10000.0f;
        m_boundaryDampening = 256.0f;
        m_speedLimiting = 500.0f;
        
        m_kernelPoly6 = 315.0f / (64.0f * PI * std::pow(m_smoothRadius, 9));
        m_kernelSpiky = -45.0f / (PI * std::pow(m_smoothRadius, 6));
        m_kernelViscosity = 45.0f / (PI * std::pow(m_smoothRadius, 6));
        
    }
    
    /**
     * @brief 初始化流体系统，设置流体范围
     * @param {short} maxPointCounts 最大流体粒子数量
     * @param {fBox3} wallBox 墙体盒子网格范围
     * @param {fBox3} initFluidBox_min 流体盒子网格范围
     * @param {glm::fvec3*} gravity 重力加速度
     */
    void FluidSystem::_init(unsigned short maxPointCounts, const fBox3& wallBox, const fBox3& initFluidBox, const glm::fvec3 &gravity){
        m_pointType.reset(maxPointCounts);
        m_gridType.init(wallBox, m_unitScale, m_smoothRadius * 2.0f, BOX_BORDER);
        
        m_sphWallBox = wallBox;
        m_gravityDir = gravity;
        //计算粒子长度，实际公式就是：质量 / 密度 = 体积，因为是假设为正方体，再开三次方
        float pointRadius = pow(m_pointMass / m_restDensity , 1.0f / 3.0f);
        this->addFluidVolume(initFluidBox, pointRadius / m_unitScale);
        //数据长度就是粒子个数*3，因为在3D场景中
        this->pointPositionData = std::vector<float>(3 * m_pointType.size(), 0);
    }
    
    /**
     * @brief 渲染数据准备
     */
    void FluidSystem::tick(){
        m_gridType.insertParticles(&m_pointType);
        this->computePressure();
        this->computeForce();
        this->advance();
    }
    /**
     * @brief 计算粒子所受压强及粒子密度
     */
    void FluidSystem::computePressure(){

        float h_2 = pow(m_smoothRadius, 2.0f);
        m_neighborTable.reset(m_pointType.size());
        for(unsigned int i = 0; i < m_pointType.size(); ++i){
            Point *p = m_pointType.get(i);
            float sum = 0.0;
            m_neighborTable.point_prepare(i);
            int gridCell[GRID_NUM_3D];
            m_gridType.findCells(p->position, m_realRadius, gridCell);
            for(int j = 0; j < GRID_NUM_3D; ++j){
                //如果不存在领接网格，直接跳到下一次
                if(!gridCell[j]){
                    continue;
                }
                int pointN = m_gridType.getGridData(gridCell[j]);
                
                while(pointN != -1){
                    Point *pN = m_pointType.get(pointN);
                    if(p == pN){
                        sum += pow(h_2, 3.0f);
                    }else{
                        glm::fvec3 pv = (p->position - pN->position) * m_unitScale;
                        float pvLength = glm::length(pv);
                        float pv_2 = pow(pvLength, 2.0);
                        if(h_2 > pv_2){
                            sum += pow((h_2 - pv_2), 3.0f);
                            //构建邻接表
                            if(!m_neighborTable.point_add_neighbor(pointN, pvLength)){
                                goto NEIGHBOR_FULL;
                            }
                        }
                    }
                    pointN = pN->next;
                }
            }
            NEIGHBOR_FULL:
                //此时邻接表已经构建完毕了，因为已经达到可以存储的最大邻居数量了，然后就将领接表提交
                m_neighborTable.point_commit();
                //计算出粒子密度
                p->density = m_kernelPoly6 * m_pointMass * sum;
                //计算公式 density = K*(pd - rd);
                p->pressure = (p->density - m_restDensity) * m_gasConstantK;
        }
        
        
    }
    /**
     * @brief 计算粒子加速度
     */
    void FluidSystem::computeForce(){
        for(int i = 0; i < m_pointType.size(); ++i){
            Point *p = m_pointType.get(i);
            glm::fvec3 accel_sum = glm::fvec3(0.0f);
            int neighCount = m_neighborTable.getNeighborCounts(i);
            for(int point = 0; point < neighCount; point++){
                unsigned short neighBorIndex;
                float r;
                m_neighborTable.getNeighborInfo(i, point, neighBorIndex, r);
                Point *pN = m_pointType.get(neighBorIndex);
                glm::fvec3 ri_rj = (p->position - pN->position) * m_unitScale;
                float h_r = m_smoothRadius - r;
                //计算压强下的作用力
                float pressure = -m_pointMass * m_kernelSpiky * pow(h_r, 2.0f) * (p->pressure + pN->pressure) / 2 * (p->density * pN->density);
                accel_sum += ri_rj / r * pressure;
                //计算粘性力
                float viscosity = m_kernelViscosity * m_viscosity * h_r * m_pointMass / (p->density * pN->density);
                accel_sum += (p->velocity_eval - pN->velocity) * viscosity;
            }
            p->accel = accel_sum;
        }
    }
    /**
     * @brief 粒子移动
     */
    void FluidSystem::advance(){
        float deltaTime = 0.003f;
        float sl_2 = std::pow(m_speedLimiting, 2.0f);
        for(unsigned int i = 0; i < m_pointType.size(); ++i){
            Point *point = m_pointType.get(i);
            //首先对粒子当前速度进行判断，如果超过了最大允许粒子运动速度，就将粒子的加速度进行限制 a = a * v_l / a
            glm::fvec3 accel = point->accel;
            float accelLength_2 = std::pow(glm::length(accel), 2.0f);
            if(accelLength_2 > sl_2){
                accel *= m_speedLimiting / sqrt(accelLength_2);
            }
            float diff;
           //边界情况
           // Z方向边界
           diff = 0.5 * m_unitScale - (point->position.z - m_sphWallBox.min.z)*m_unitScale;
           if (diff > 0.0f )
           {
               glm::vec3 norm(0, 0, 1.0);
               float adj = m_boundartStiffness * diff - m_boundaryDampening * glm::dot ( norm,point->velocity_eval );
               accel.x += adj * norm.x;
               accel.y += adj * norm.y;
               accel.z += adj * norm.z;
           }

           diff = 0.5 * m_unitScale - (m_sphWallBox.max.z - point->position.z)*m_unitScale;
           if (diff > 0.0f)
           {
               glm::vec3 norm( 0, 0, -1.0);
               float adj = m_boundartStiffness * diff - m_boundaryDampening *glm::dot ( norm,point->velocity_eval );
               accel.x += adj * norm.x;
               accel.y += adj * norm.y;
               accel.z += adj * norm.z;
           }

           //X方向边界
           diff = 0.5 * m_unitScale - (point->position.x - m_sphWallBox.min.x)*m_unitScale;
           if (diff > 0.0f )
           {
               glm::vec3 norm(1.0, 0, 0);
               float adj = m_boundartStiffness * diff - m_boundaryDampening * glm::dot ( norm,point->velocity_eval ) ;
               accel.x += adj * norm.x;
               accel.y += adj * norm.y;
               accel.z += adj * norm.z;
           }

           diff = 0.5 * m_unitScale - (m_sphWallBox.max.x - point->position.x)*m_unitScale;
           if (diff > 0.0f)
           {
               glm::vec3 norm(-1.0, 0, 0);
               float adj = m_boundartStiffness * diff - m_boundaryDampening * glm::dot ( norm,point->velocity_eval );
               accel.x += adj * norm.x;
               accel.y += adj * norm.y;
               accel.z += adj * norm.z;
           }

           //Y方向边界
           diff = 0.5 * m_unitScale - ( point->position.y - m_sphWallBox.min.y )*m_unitScale;
           if (diff > 0.0f)
           {
               glm::vec3 norm(0, 1.0, 0);
               float adj = m_boundartStiffness * diff - m_boundaryDampening * glm::dot ( norm,point->velocity_eval );
               accel.x += adj * norm.x;
               accel.y += adj * norm.y;
               accel.z += adj * norm.z;
           }
           diff = 0.5 * m_unitScale - ( m_sphWallBox.max.y - point->position.y )*m_unitScale;
           if (diff > 0.0f)
           {
               glm::vec3 norm(0, -1.0, 0);
               float adj = m_boundartStiffness * diff - m_boundaryDampening * glm::dot ( norm,point->velocity_eval );
               accel.x += adj * norm.x;
               accel.y += adj * norm.y;
               accel.z += adj * norm.z;
           }

            //加速度需要加上重力加速度，因为会收到重力影响
            accel += m_gravityDir;
            std::cout << accel.y << std::endl;
            glm::fvec3 vNext = point->velocity + accel * deltaTime;
            point->velocity_eval = (point->velocity + vNext) * 0.5f;
            glm::fvec3 distance = vNext * deltaTime / m_unitScale;
            point->position += distance;
            point->velocity = vNext;
            
            pointPositionData[3 * i] = point->position.x;
            pointPositionData[3 * i + 1] = point->position.y;
            pointPositionData[3 * i + 2] = point->position.z;
        }
    }
    /**
     * @brief 创建初始液体块
     * @param {fBox3} fluidBox 流体盒子
     * @param {float} spacing 网格间隔
     */
    void FluidSystem::addFluidVolume(const fBox3 &fluidBox, float spacing){
        for(float z = fluidBox.max.z; z >= fluidBox.min.z; z -= spacing){
            for(float y = fluidBox.min.y; y < fluidBox.max.y; y += spacing){
                for(float x = fluidBox.min.x; x < fluidBox.max.y; x += spacing){
                    
                    Point* point = m_pointType.addPointReuse();
                    point->position = glm::fvec3(x, y, z);
                }
            }
        }
        

    }
    
    FluidSystem::~FluidSystem(){}
}

