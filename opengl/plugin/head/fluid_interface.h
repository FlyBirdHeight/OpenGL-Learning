//
//  fluid_interface.h
//  opengl
//
//  Created by adsionli on 2021/6/18.
//  Copyright © 2021 adsionli. All rights reserved.
//

#ifndef fluid_interface_h
#define fluid_interface_h

#include "sph_stdafx.hpp"
#include "point_type.hpp"

namespace SPH {
    /**
     * @brief 流体系统接口类，均是纯虚函数，派生类自己实现其中方法
     */
    class System{
        public:
            virtual void init(unsigned short maxPointCounts,
                              const glm::fvec3* wallBox_min, const glm::fvec3* wallBox_max,
                              const glm::fvec3* initFluidBox_min, const glm::fvec3* initFluidBox_max,
                              const glm::fvec3* gravity) = 0;
            virtual unsigned int getPointStride(void)  const = 0;
            virtual unsigned int getPointCounts(void) const = 0;
            virtual const Point* getPointBuf(void) const = 0;
            virtual void tick(void) = 0;
    };
}
#endif /* fluid_interface_h */
