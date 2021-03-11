#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aNormal;
layout(location = 2) in vec3 aColor;
layout(location = 3) in vec3 aOffset;

struct Light {
    in vec3 ambient;
    in vec3 diffuse;
    in vec3 specular;
    in vec3 direction;
}

uniform Light light;
uniform vec3 viewPos;
uniform mat4 model;
uniform mat4 projection;
uniform mat4 viewer;

out vec3 flatColor;
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
    
    float spec = pow(max(dot(viewPos, halfDir[p]), 0.0), 16.0);
    vec3 specular = light.specular * spec;
    
    return (ambient * diffuse * specular);
}

void main(){
    vec3 FragPos = vec3(model * vec4(aPos + aOffset, 1.0));
    vec3 Normal = aNormal;
    
    vec3 lighting = calculateLighting(Normal, FragPos);
    Color = aColor * lighting;
    fragColor = Color;
    
    gl_Position = projection * viewer * model * vec4(aPos + aOffset, 1.0);
}

