#version 330 core
layout (location = 0) in vec3 aPos;
uniform vec4 model;
uniform vec4 view;
uniform vec4 projection;

void main(){
    gl_Position = projection * view * model * vec4(aPos,1.0);
}
