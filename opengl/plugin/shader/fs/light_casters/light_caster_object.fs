#version 330 core
struct Material{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
struct Light{
    vec3 direction;
	vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	//常数项
	float constant;
    //线性改变,一次项
	float linear;
    //二次项
	float quadratic;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
  
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

out vec4 FragColor;

void main()
{
    vec3 norm = normalize(Normal);
	//光线方向是由光源到物体，但是在定义中是物体到光源，所以需要取反
    //vec3 lightDir = normalize(-light.direction);
	vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	
    vec3 ambient  = light.ambient * texture(material.diffuse, TexCoords).rgb;
    vec3 diffuse  = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;  
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb; 

	//在片段作色器中计算衰减，并处理环境光、漫反射光、镜面反射光
	float distance = length(light.position - FragPos);
	float attenuation = 1.0/ (light.constant + light.linear * distance + light.quadratic * pow(distance,2));
	ambient *= attenuation;
	diffuse  *= attenuation;
	specular *= attenuation;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
