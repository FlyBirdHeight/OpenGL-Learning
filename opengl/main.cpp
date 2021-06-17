#include <iostream>
#include <string>
#include <typeinfo>
struct PointData {
    float density;    //密度
    float pressure;    //压力
    PointData(): density(0), pressure(0){}
};
int main(){
    PointData* b;
    b = (PointData*)malloc(1000 * sizeof(PointData));
    for(int i = 0; i < 1000; ++i){
        PointData* x = b + i;
        x->density = (float)i;
        x->pressure = (float)i;
    }
    
    
    std::cout << (b+99)->density << "  " << (b + 99)->pressure << std::endl;
 
    return 0;
}
