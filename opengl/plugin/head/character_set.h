#ifndef character_set_h
#define character_set_h


#include <math.h>
#include <iostream>
#include <string>

class CharacterSet {
public:
    CharacterSet()
    {
        x0          =   0;
        y0          =   0;
        x1          =   0;
        y1          =   0;
        offsetX     =   0;
        offsetY     =   0;
    }
    /**
    *   存储当前字符在纹理上的坐标位置
    */
    
    unsigned int   x0:10;
    unsigned int   y0:10;
    unsigned int   x1:10;
    unsigned int   y1:10;
    unsigned int   offsetX:10;
    unsigned int   offsetY:10;
    unsigned int   textureId;
};

#endif
