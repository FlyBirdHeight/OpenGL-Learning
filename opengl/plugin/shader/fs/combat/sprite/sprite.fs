#version 330 core
in vec2 TexCoords;

uniform sampler2D image;
uniform vec3 spriteColor;

out vec4 FragColor;

void main(){
    FragColor = vec4(spriteColor, 1.0f) * texture(image, TexCoords);
}
