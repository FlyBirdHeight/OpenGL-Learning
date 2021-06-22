//
//  sph_show.hpp
//  opengl
//
//  Created by adsionli on 2021/6/21.
//  Copyright © 2021 adsionli. All rights reserved.
//

#ifndef sph_show_hpp
#define sph_show_hpp

#include <stdio.h>
#include "sph_stdafx.hpp"
#include "fluid_system.hpp"
#include "shaders.h"
#include "camera.h"
using namespace SPH;
class SphShow{
public:
    SphShow();
    void render();
private:
    GLFWwindow* initWindow();
    void setPointData();
    void setBoxData();
    unsigned int pointVao = 0, pointVbo = 0, boxVao = 0, boxVbo = 0, boxEbo = 0;
    SPH::FluidSystem fluidsystem;
    glm::fvec3 wallBoxMin = glm::fvec3(0.0, 0.0, 0.0);
    glm::fvec3 wallBoxMax = glm::fvec3(20.0, 20.0, 20.0);
    glm::fvec3 fluidBoxMin = glm::fvec3(2.0, 2.0, 2.0);
    glm::fvec3 fluidBoxMax = glm::fvec3(18.0, 18.0, 18.0);
    glm::fvec3 gravity = glm::fvec3(.0, -9.8, .0);
    unsigned int pointCount = POINT_COUNT;
    
    std::string sphVs = "./shader/vs/sph/first/sph.vs";
    std::string sphFs = "./shader/fs/sph/first/sph.fs";
};

#endif /* sph_show_hpp */
