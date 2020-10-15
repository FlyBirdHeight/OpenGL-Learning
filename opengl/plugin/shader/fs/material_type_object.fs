#version 330 core
out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
    vec3 ambient = light.ambient * material.ambient;
    //计算定向的光线向量，使用光照的方向向量减去法线向量，同时为了之后算射入角的大小方便，要把它进行标准化。
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    //dot 向量点乘运算 ， 结果取最大值输出， 两个向量之间的角度大于90度，点乘的结果就会变成负数
    float diff = max(dot(norm, lightDir), 0.0);
    //获取漫反射分量
    vec3 diffuse = light.diffuse * (diff * material.diffuse);
    
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir,norm);
    float spec = pow(max(dot(viewDir,reflectDir),0.0), material.shininess);
    vec3 specular =  light.specular * (spec * material.specular);
    
    vec3 result = ambient + diffuse + specular;
    
    FragColor = vec4(result,1.0);
}

