#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
//法线
out vec3 Normal;
//位置向量
out vec3 FragPos;
//纹理坐标
out vec2 Texcoords;

void main()
{
    FragPos = vec3(model * vec4(aPos,1.0));
    Normal = aNormal;
    Texcoords = vec2(aTexcoord.x,aTexcoord.y);
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}


