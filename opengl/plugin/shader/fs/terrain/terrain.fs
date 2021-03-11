#version 330 core
in vec3 fragColor;
in vec3 Color;
out vec4 FragColor;

uniform bool iFrame;


void main(){
    if(iFrame){
        FragColor = fragColor;
    }else{
        FragColor = Color;
    }
}
