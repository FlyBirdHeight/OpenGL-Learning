#include <stdio.h>
#include <math.h>
#include <vector>
#include <thread>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace std;
struct Circle{
    int x;
    int y;
    Circle(int xC, int yC): x(xC), y(yC){
        
    }
};
std::vector<Circle> circles;

static void setCircle(const Circle &circle){
    circles.push_back(circle);
}

static void setSymmetricData(int x, int y, int xC, int yC){
    setCircle(Circle(xC + x, yC + y));
    setCircle(Circle(xC - x, yC + y));
    setCircle(Circle(xC + x, yC - y));
    setCircle(Circle(xC - x, yC - y));
    setCircle(Circle(xC + y, yC + x));
    setCircle(Circle(xC - y, yC + x));
    setCircle(Circle(xC + y, yC - x));
    setCircle(Circle(xC - y, yC - x));
}

static void draw(int xC, int yC, int radius){
    int p = 1 - radius;
    setSymmetricData(0, radius, xC, yC);
    int x = xC, y = radius;
    for(int i = xC; xC < yC; ++i){
        x++;
        if(p < 0){
            p += 2 * x + 1;
        }else{
            y--;
            p += 2 * (x - y) + 1;
        }
        setSymmetricData(x, y, xC, yC);
    }
}
