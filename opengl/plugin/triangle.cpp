//三角形类
#include "head/triangle.h"

void Triangle::processInput(GLFWwindow *window){
    //glfwGetKey -> 判断是否存在键盘输入事件 GLFW_KEY_ESCAPE -> 输入为esc
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        //glfwSetWindowShouldClose -> 指定关闭相应窗口
        glfwSetWindowShouldClose(window, true);
    }
}
void Triangle::setVertex(float *data){
    vertices = data;
}
void Triangle::create(){
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
    
    //索引缓冲对象
    float vertices[] = {
        0.5f, 0.5f, 0.0f,   // 右上角
        0.5f, -0.5f, 0.0f,  // 右下角
        -0.5f, -0.5f, 0.0f, // 左下角
        -0.5f, 0.5f, 0.0f   // 左上角
    };

    unsigned int indices[] = { // 注意索引从0开始!
        0, 1, 3, // 第一个三角形
        1, 2, 3  // 第二个三角形
    };
    //创建索引缓冲对象
    glGenBuffers(1, &EBO);

    //创建顶点数组对象VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    //一个缓冲ID生成一个VBO对象
    glGenBuffers(1, &VBO);
    //绑定一个顶点缓冲对象绑定到GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //使用的任何（在GL_ARRAY_BUFFER目标上的）缓冲调用都会用来配置当前绑定的缓冲(VBO)。然后我们可以调用glBufferData函数，它会把之前定义的顶点数据复制到缓冲的内存中
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //绑定索引缓冲对象
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    
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
    //顶点数据解析
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //着色器程序对象激活
    glUseProgram(shaderProgram);
    //删除着色器
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    while(!glfwWindowShouldClose(window))
    {
        
        //加入注册事件(键盘输入esc事件)来主动终止循环渲染
        processInput(window);
        /*
         渲染指令在循环渲染中生效
         glClearColor函数是一个状态设置函数，而glClear函数则是一个状态使用的函数，它使用了当前的状态来获取应该清除为的颜色。
         glClearColor设置清空屏幕所使用的颜色
         glClear函数来清空屏幕的颜色缓冲，可以指定接受一个缓冲位(Buffer Bit)来指定要清空的缓冲，可能的缓冲位有GL_COLOR_BUFFER_BIT，GL_DEPTH_BUFFER_BIT和GL_STENCIL_BUFFER_BIT
         */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //绘制物体
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        //检查并调用事件，交换缓冲
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
}
