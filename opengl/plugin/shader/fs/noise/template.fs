#define Use_Perlin
//#define Use_Value
//#define Use_Simplex

// ========= Noise ===========
vec2 hash22(vec2 p)
{
    //伪随机梯度向量生成
    p = vec2( dot(p,vec2(127.1,311.7)),
              dot(p,vec2(269.5,183.3)));
    
    //return normalize(-1.0 + 2.0 * fract(sin(p)*43758.5453123));
    //归一化
    return -1.0 + 2.0 * fract(sin(p)*43758.5453123);
}

float hash21(vec2 p)
{
    //伪随机值生成
    float h = dot(p,vec2(127.1,311.7));
    
    //归一化处理
    return -1.0 + 2.0 * fract(sin(h)*43758.5453123);
}

float perlin_noise(vec2 p)
{
    //晶格左下角顶点
    vec2 pi = floor(p);
    //指向p点的向量
    vec2 pf = p - pi;
    //缓和曲线s(t) = 3t^2 - 2t^3
    vec2 w = pf * pf * (3.0 - 2.0 * pf);
    //线性混合
    return mix(mix(dot(hash22(pi + vec2(0.0, 0.0)), pf - vec2(0.0, 0.0)),
                   dot(hash22(pi + vec2(1.0, 0.0)), pf - vec2(1.0, 0.0)), w.x),
               mix(dot(hash22(pi + vec2(0.0, 1.0)), pf - vec2(0.0, 1.0)),
                   dot(hash22(pi + vec2(1.0, 1.0)), pf - vec2(1.0, 1.0)), w.x),
               w.y);
}

float value_noise(vec2 p)
{
    //晶格左下角顶点
    vec2 pi = floor(p);
    //指向p点的向量
    vec2 pf = p - pi;
    //缓和曲线s(t) = 3t^2 - 2t^3
    vec2 w = pf * pf * (3.0 - 2.0 * pf);
    
    return mix(mix(hash21(pi + vec2(0.0, 0.0)), hash21(pi + vec2(1.0, 0.0)), w.x),
               mix(hash21(pi + vec2(0.0, 1.0)), hash21(pi + vec2(1.0, 1.0)), w.x),
               w.y);
}

float simplex_noise(vec2 p)
{
    const float K1 = 0.366025404; // (sqrt(3)-1)/2;
    const float K2 = 0.211324865; // (3-sqrt(3))/6;
    
    vec2 i = floor(p + (p.x + p.y) * K1);
    
    vec2 a = p - (i - (i.x + i.y) * K2);
    vec2 o = (a.x < a.y) ? vec2(0.0, 1.0) : vec2(1.0, 0.0);
    vec2 b = a - (o - K2);
    vec2 c = a - (1.0 - 2.0 * K2);
    
    vec3 h = max(0.5 - vec3(dot(a, a), dot(b, b), dot(c, c)), 0.0);
    vec3 n = h * h * h * h * vec3(dot(a, hash22(i)), dot(b, hash22(i + o)), dot(c, hash22(i + 1.0)));
    
    return dot(vec3(70.0, 70.0, 70.0), n);
}

float noise(vec2 p) {
#ifdef Use_Perlin
    return perlin_noise(p);
#elif defined Use_Value
    return value_noise(p);
#elif defined Use_Simplex
    return simplex_noise(p);
#endif
    
    return 0.0;
}

// ========== Different function ==========
//fbm分型布朗运动
float noise_itself(vec2 p)
{
    return noise(p * 8.0);
}

float noise_sum(vec2 p)
{
    float f = 0.0;
    p = p * 4.0;
    f += 1.0000 * noise(p); p = 2.0 * p;
    f += 0.5000 * noise(p); p = 2.0 * p;
    f += 0.2500 * noise(p); p = 2.0 * p;
    f += 0.1250 * noise(p); p = 2.0 * p;
    f += 0.0625 * noise(p); p = 2.0 * p;
    
    return f;
}

float noise_sum_abs(vec2 p)
{
    float f = 0.0;
    p = p * 7.0;
    f += 1.0000 * abs(noise(p)); p = 2.0 * p;
    f += 0.5000 * abs(noise(p)); p = 2.0 * p;
    f += 0.2500 * abs(noise(p)); p = 2.0 * p;
    f += 0.1250 * abs(noise(p)); p = 2.0 * p;
    f += 0.0625 * abs(noise(p)); p = 2.0 * p;
    
    return f;
}

float noise_sum_abs_sin(vec2 p)
{
    float f = noise_sum_abs(p);
    f = sin(f * 1.5 + p.x * 7.0);
    
    return f * f;
}

// ========== Draw ==========
//绘制简单模型
vec3 draw_simple(float f)
{
    //归一化处理
    f = f * 0.5 + 0.5;
    return f * vec3(25.0/255.0, 161.0/255.0, 245.0/255.0);
}
//绘制云
vec3 draw_cloud(float f)
{
    //归一化处理
    f = f * 0.5 + 0.5;
    return mix(    vec3(8.0/255.0, 65.0/255.0, 82.0/255.0),
                  vec3(178.0/255.0, 161.0/255.0, 205.0/255.0),
                   f*f);
}
//绘制火焰
vec3 draw_fire(float f)
{
    //归一化处理
    f = f * 0.5 + 0.5;
    returmix(    vec3(131.0/255.0, 8.0/255.0, 0.0/255.0),
             vec3(204.0/255.0, 194.0/255.0, 56.0/255.0),
              pow(f, 3.));
}
//绘制大理石纹理
vec3 draw_marble(float f)
{
    //归一化处理
    f = f * 0.5 + 0.5;
    return mix(    vec3(31.0/255.0, 14.0/255.0, 4.0/255.0),
                  vec3(172.0/255.0, 153.0/255.0, 138.0/255.0),
                   1.0 - pow(f, 5.));
}

//绘制圆，就是实例中的中心位置的圆形
vec3 draw_circle(vec2 p, float radius, vec3 col, vec3 back)
{
    p = 2.0 * p - vec2(iResolution.x/iResolution.y, 1.0);
    return     mix(col, back, smoothstep(0.0, 0.01, length(p) - radius)) *
            smoothstep(0.0, 0.02, abs(length(p) - radius));
            
}
//主要图片的输出
void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 p = fragCoord.xy / iResolution.xy;
    vec2 uv = p * vec2(iResolution.x/iResolution.y, 1.0);
    vec2 split = vec2(0.5, 0.5);
    if (iMouse.z > 0.0) {
        split += 2.0 * iMouse.xy/iResolution.xy - 1.0;
    }
    
    float f = 0.0;
    vec3 col = vec3(0.0, 0.0, 0.0);
    
    if (p.x < split.x && p.y > split.y) {
        f = noise_itself(uv);
        col = draw_simple(f);
    } else if (p.x < split.x && p.y <= split.y) {
        f = noise_sum(uv);
        col = draw_cloud(f);
    } else if (p.x >= split.x && p.y < split.y) {
        f = noise_sum_abs(uv);
        col = draw_fire(f);
    } else {
        f = noise_sum_abs_sin(uv);
        col = draw_marble(f);
    }
    
    f = f * 0.5 + 0.5;
    col = draw_circle(uv, 0.9, col, vec3(f));
    col = mix(vec3(0.3, 0.0, 0.0), col, smoothstep(0.0, 0.005, abs(p.x - split.x)));
    col = mix(vec3(0.3, 0.0, 0.0), col, smoothstep(0.0, 0.005*iResolution.x/iResolution.y, abs(p.y - split.y)));
    
    fragColor = vec4(col, 1.0);
}
