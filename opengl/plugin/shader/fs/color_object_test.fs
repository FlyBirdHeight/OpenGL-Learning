#version 330 core
out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos;
uniform vec3 objectColor;
uniform vec3 lightColor;
//光源位置
uniform vec3 lightPos;
//观察位置
uniform vec3 viewPos;

void main()
{
    //环境常数，计算环境光照分量
    float ambientStrength = 0.1;
    //设置镜面强度
    float specularStrenght = 0.5;
    
    vec3 ambient = ambientStrength * lightColor;
    //计算定向的光线向量，使用光照的方向向量减去法线向量，同时为了之后算射入角的大小方便，要把它进行标准化。
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    //dot 向量点乘运算 ， 结果取最大值输出， 两个向量之间的角度大于90度，点乘的结果就会变成负数
    float diff = max(dot(norm, lightDir), 0.0);
    //获取漫反射分量
    vec3 diffuse = diff * lightColor;
    
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir,norm);
    float spec = pow(max(dot(viewDir,reflectDir),0.0),32);
    vec3 specular = specularStrenght * spec * lightColor;
    vec3 result = (ambient + diffuse + specular) * objectColor;
    
    FragColor = vec4(result,1.0);
}
