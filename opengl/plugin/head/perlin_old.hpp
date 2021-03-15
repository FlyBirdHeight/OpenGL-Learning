#ifndef perlin_old_hpp
#define perlin_old_hpp
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <random>
#include <cmath>
class PerlinNoiseOld{
    //定义纹理大小
    static const unsigned int g_tableSize = 256;
    //定义随机排列数组大小
    static const unsigned int p_tableSize = g_tableSize - 1;
public:
    PerlinNoiseOld();
    //生成晶格数据
    void generateMesh();
    //生成对应的晶格数据
    float generateMeshData(glm::vec2 point);
    //缓和曲线
    float smoothCurve(float data);
    //梯度向量下标获取
    glm::fvec2 getIndex(int x, int y);
    //测试生成晶格
    void testHash(int x, int y);
    float getNoiseByFbm(int octaves, float x, float y);
private:
    unsigned int SCR_WIDTH = 800, SCR_HEIGHT = 600;
    //定义查找表
    glm::fvec2 gradient[g_tableSize];
    //定义随机排列数组
    unsigned permutationTable[g_tableSize * 2];
    
};

#endif /* perlin_old_hpp */
