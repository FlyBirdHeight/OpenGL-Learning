//
//  show.hpp
//  opengl
//
//  Created by adsionli on 2021/6/23.
//  Copyright © 2021 adsionli. All rights reserved.
//

#ifndef show_hpp
#define show_hpp

#include <stdio.h>
#include "stdafx.hpp"
#include "fluid.hpp"
#include "shaders.h"
#include "camera.h"
class SphShow{
public:
    SphShow();
    void render();
private:
    GLFWwindow* initWindow();
    void setPointData();
    void setBoxData();
    unsigned int pointVao = 0, pointVbo = 0, boxVao = 0, boxVbo = 0, boxEbo = 0;
    FluidSystem fluidsystem;
    glm::fvec3 wallBoxMin = glm::fvec3(0.0, 0.0, 0.0);
    glm::fvec3 wallBoxMax = glm::fvec3(20.0, 20.0, 20.0);
    glm::fvec3 fluidBoxMin = glm::fvec3(3.0, 3.0, 3.0);
    glm::fvec3 fluidBoxMax = glm::fvec3(17.0, 10.0, 17.0);
    glm::fvec3 gravity = glm::fvec3(.0, -9.8, .0);
    unsigned int pointCount = 4096*2;
    
    std::string sphVs = "./shader/vs/sph/first/sph.vs";
    std::string sphFs = "./shader/fs/sph/first/sph.fs";
    
    std::string sphPointVs = "./shader/vs/sph/first/sph_point.vs";
    std::string sphPointFs = "./shader/fs/sph/first/sph_point.fs";

};

#endif /* show_hpp */
