#version 330 core

in vec2 TexCoords;
uniform float iResolution;
out vec4 FragColor;
float gradientVector(vec2 p){
    p = vec2(dot(p,vec2(127.1,311.7)), dot(p,vec2(269.5,183.3)));
    
    return -1. + 2. * fract(sin(p) * 43758.5453123);
}

float noise_sum_abs(vec2 p)
{
    float f = 0.0;
    p = p * 7.0;
    f += 1.0000 * abs(perlin_noise(p)); p = 2.0 * p;
    f += 0.5000 * abs(perlin_noise(p)); p = 2.0 * p;
    f += 0.2500 * abs(perlin_noise(p)); p = 2.0 * p;
    f += 0.1250 * abs(perlin_noise(p)); p = 2.0 * p;
    f += 0.0625 * abs(perlin_noise(p)); p = 2.0 * p;
    
    return f;
}

float perlin_noise(vec2 p){
    //晶格左下角顶点
    vec2 pi = floor(p);
    //指向p点的向量
    vec2 pf = p - pi;
    //缓和曲线s(t) = 3t^2 - 2t^3
    vec2 w = pf * pf * (3.0 - 2.0 * pf);
    //双线性插值
    return mix(mix(dot(gradientVector(pi + vec2(0.0, 0.0), pf - vec2(0.0, 0.0)),          dot(gradientVector(pi + vec2(1.0，0.0)， pf - vec2(1.0, 0.0)),w.x),
               mix(dot(gradientVecor(pi + vec2(0.0, 1.0)), pf - vec2(0.0, 1.0)),dot(gradientVector(pi + vec2(1.0, 1.0)), pf - vec2(1.0, 1.0)) , w.x), w.y);
}
                   
vec3 draw_fire(float f){
    f = f * 0.5 + 0.5;
        
    return mix(vec3(131.0/255.0, 8.0/255.0, 0.0/255.0),vec3(204.0/255.0, 194.0/255.0, 56.0/255.0),pow(f, 3.));
}

void main(){
    vec2 p = TexCoords.xy / iResolution;
    vec2 uv = p * vec2(iResolution.x / iResolution.y, 1.0);
    vec2 split = vec2(.5, .5);
    float f = 0.0;
    vec3 col = vec3(0.0, 0.0, 0.0);
    f = noise_sum_abs(uv);
    col = draw_fire(f);
    
    FragColor = vec4(col, 1.0);
}
