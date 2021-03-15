#version 330 core
flat in vec3 flatColor;
in vec3 Color;

out vec4 FragColor;

uniform bool iFrame;

void main() {
    if (iFrame) {
        FragColor = vec4(flatColor, 1.0);
    } else {
        FragColor = vec4(Color, 1.0);
    }
//    FragColor = vec4(0.0, 0.0, 0.0, 1.0);
}
