#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aColor;
layout(location = 3) in vec3 aOffset;

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 direction;
};

uniform Light light;
uniform vec3 viewPos;
uniform mat4 model;
uniform mat4 projection;
uniform mat4 viewer;

out vec3 fragColor;
out vec3 Color;

vec3 calculateLighting(vec3 Normal, vec3 FragPos){
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(light.direction - FragPos);
    
    vec3 ambient = light.ambient;
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * light.diffuse;
    
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfDir = normalize(viewDir + lightDir);
    
    float spec = pow(max(dot(viewPos, halfDir), 0.0), 16.0);
    vec3 specular = light.specular * spec;
    
    return (ambient * diffuse * specular);
}

void main(){
    vec3 FragPos = vec3(model * vec4(aPos + aOffset, 1.0));
    vec3 Normal = aNormal;
    
    vec3 lighting = calculateLighting(Normal, FragPos);
    Color = aColor;
    fragColor = aColor * lighting;
    
    gl_Position = projection * viewer * model * vec4(aPos + aOffset, 1.0);
}

