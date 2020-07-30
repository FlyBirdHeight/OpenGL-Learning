#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

int main()
{
    //编译GLSL顶点着色器
    const char*vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";
    //编译片段着色器
    const char *fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\n\0";
    glfwInit();
    //主版本号
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //副版本号
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //profile opengl使用模式core profile核心模式
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //forward_compat向前兼容
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    //双缓冲设置
    glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
    //创建glfw窗口，第一个参数宽度，第二个参数高度，第三个参数是窗口名称，glfwCreateWindow返回的是GLFWwindow对象
     GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
     if (window == NULL)
     {
         printf("创建成功");
         glfwTerminate();
     }
     printf("创建成功");
     glfwMakeContextCurrent(window);
     //glad用来管理opengl中的函数指针
     if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
     {
         printf("Failed to initialize GLAD");
     }
    
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };
    unsigned int VBO;
    unsigned int vertexShader;
    //一个缓冲ID生成一个VBO对象
    glGenBuffers(1, &VBO);
    //绑定一个顶点缓冲对象绑定到GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //使用的任何（在GL_ARRAY_BUFFER目标上的）缓冲调用都会用来配置当前绑定的缓冲(VBO)。然后我们可以调用glBufferData函数，它会把之前定义的顶点数据复制到缓冲的内存中
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //创建一个顶点着色器
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //glShaderSource函数把要编译的着色器对象作为第一个参数。第二参数指定了传递的源码字符串数量，这里只有一个。第三个参数是顶点着色器真正的源码
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    //判断编译是否成功，如果编译失败，使用glGetShaderInfoLog函数来捕获失败信息
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    //编译片段着色器
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    //创建着色器程序对象,glCreateProgram函数创建一个程序，并返回新创建程序对象的ID引用。
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    //建立着色器与着色器程序对象之间的连线
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::Link\n" << infoLog << std::endl;
    }
    
    return 0;
}
