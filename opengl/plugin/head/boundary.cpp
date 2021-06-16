//
//  boundary.cpp
//  opengl
//
//  Created by adsionli on 2021/6/11.
//  Copyright © 2021 adsionli. All rights reserved.
//

#include <iostream>
#include <cassert>
#include <algorithm>
#include <string>
#include <list>
#include <deque>
#include <vector>
#define RGBA 4
using namespace std;
struct BoundaryData {
    int x;
    int y;
    std::vector<int> color;
    std::vector<int> borderColor;
    BoundaryData(int x,int y,std::vector<int> color,std::vector<int> borderColor): x(x), y(y){
        color.assign(color.begin(), color.end());
        borderColor.assign(borderColor.begin(), borderColor.end());
    }
};
class BoundaryCal{
public:
    std::vector<BoundaryData> BoundaryFill(int x, int y, std::vector<int> fillColor, std::vector<int> borderColor){
        int interColor[RGBA];
        BoundaryData data = returnData[count];
        if(!std::equal(data.color.begin(), data.color.end(), fillColor.begin()) && !std::equal(data.borderColor.begin(), data.borderColor.end(), borderColor.begin())){
            count++;
        }

        return returnData;
    }
private:
    int count = 0;
    std::vector<BoundaryData> returnData;
};
