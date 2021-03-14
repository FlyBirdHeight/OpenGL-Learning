#version 330 core
in vec3 fragColor;
in vec3 Color;
out vec4 FragColor;

uniform bool iFrame;


void main(){
    if(iFrame){
        FragColor = vec4(fragColor, 1.0);
    }else{
        FragColor = vec4(Color, 1.0);
    }
}
