//
//  sph_stdafx.hpp
//  opengl
//
//  Created by adsionli on 2021/6/16.
//  Copyright © 2021 adsionli. All rights reserved.
//

#ifndef sph_stdafx_hpp
#define sph_stdafx_hpp

#include <stdio.h>
#include <vector>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <map>
#include <list>
#include <set>
#include <stack>
#include <algorithm>
#include <assert.h>

enum {
    //最大容器粒子数
    ELEM_MAX = 4096,
    //粒子邻接表最大记录邻居数量
    MAX_NEIGHTBOR_COUNTS=80,
    //3D下光滑核采样是囊括的网格数量
    GRID_NUM_3D = 8,
    //2D下光滑核采样是囊括的网格数量
    GRID_NUM_2D = 4,
    //盒子边界大小
    BOX_BORDER = 0,
    //粒子数量
    POINT_COUNT = 4096 * 2
    
};


#endif /* sph_stdafx_hpp */
