//三角形类
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <string>
unsigned int VBO;
unsigned int vertexShader;
//编译GLSL顶点着色器
const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";
class Triangle
{
    public:
        void setVertex(float data[]){
            vertices = data;
        }
        void create(){
            //一个缓冲ID生成一个VBO对象
            glGenBuffers(1, &VBO);
            //绑定一个顶点缓冲对象绑定到GL_ARRAY_BUFFER
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            //使用的任何（在GL_ARRAY_BUFFER目标上的）缓冲调用都会用来配置当前绑定的缓冲(VBO)。然后我们可以调用glBufferData函数，它会把之前定义的顶点数据复制到缓冲的内存中
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
            //创建一个顶点着色器
            vertexShader = glCreateShader(GL_VERTEX_SHADER);
            //glShaderSource函数把要编译的着色器对象作为第一个参数。第二参数指定了传递的源码字符串数量，这里只有一个。第三个参数是顶点着色器真正的源码，第四个参数我们先设置为NULL
            glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
            glCompileShader(vertexShader);
            //判断编译是否成功，如果编译失败，使用glGetShaderInfoLog函数来捕获失败信息
            int  success;
            char infoLog[512];
            glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
            if(!success)
            {
                glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
                ;
            }
        }
    protected:
        float *vertices;
};
