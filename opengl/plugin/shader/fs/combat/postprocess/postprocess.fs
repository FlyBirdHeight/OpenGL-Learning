#version 330 core

in vec2 TexCoords;
out vec4 color;

uniform sampler2D scene;
//偏移量
uniform vec2 offsets[9];
//边缘检测卷积核
uniform int edge_kernel[9];
//模糊量(去九个像素点，平均)
uniform float blur_kernel[9];


uniform bool chaos;
uniform bool confuse;
uniform bool shake;

void main(){
    color = vec4(0.0f);
    vec3 newPosition[9];
    if(chaos || shake){
        //这里就是设置一下9个像素点的偏移量之后的纹理坐标
        for(int i = 0; i < 9; i++){
            newPosition[i] = vec3(texture(scene, TexCoords.st + offsets[i]));
        }
    }
    if(chaos){
        //将颜色去进行边缘检测卷积核
        for(int i = 0; i < 9; i++){
            color += vec4(newPosition[i] * edge_kernel[i], 0.0f);
        }
        color.a = 1.0f;
    }
    else if(confuse){
        color = vec4(1.0 - texture(scene, TexCoords).rgb, 1.0);
    }else if(shake){
        //这里就是将颜色进行平均模糊
        for(int i = 0; i < 9; i++){
            color += vec4(newPosition[i] * blur_kernel[i], 0.0f);
        }
        color.a = 1.0f;
    }else{
        color = texture(scene, TexCoords);
    }
}
