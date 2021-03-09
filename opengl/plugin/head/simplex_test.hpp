//
//  simplex_test.hpp
//  opengl
//
//  Created by adsionli on 2021/3/9.
//  Copyright © 2021 adsionli. All rights reserved.
//

#ifndef simplex_test_hpp
#define simplex_test_hpp
#include <iostream>
#include <cstddef>
class SimplexNoiseTest {
public:
    // 3D Perlin simplex noise
    static float noise(float x, float y, float z);

    float fractal(size_t octaves, float x, float y, float z) const;
    
    //测试
    void test();

    explicit SimplexNoiseTest(float frequency = 1.0f,
                          float amplitude = 1.0f,
                          float lacunarity = 2.0f,
                          float persistence = 0.5f) :
        mFrequency(frequency),
        mAmplitude(amplitude),
        mLacunarity(lacunarity),
        mPersistence(persistence) {
    }

private:
    float mFrequency;
    float mAmplitude;
    float mLacunarity;
    float mPersistence;
};
#endif /* simplex_test_hpp */
