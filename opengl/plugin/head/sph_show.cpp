////
////  sph_show.cpp
////  opengl
////
////  Created by adsionli on 2021/6/21.
////  Copyright © 2021 adsionli. All rights reserved.
////
//
//#include "sph_show.hpp"
//enum {
//    SCR_HEIGHT = 600,
//    SCR_WIDTH = 800
//};
//static void processInput(GLFWwindow *window);
//static void mouse_callback(GLFWwindow *window, double xpos, double ypos);
//static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
////相机类
//static Camera camera(glm::vec3(10.0, 10.0, 45.0));
////是否第一次点击
//static bool firstMouse = true;
//static float lastX = SCR_WIDTH / 2;
//static float lastY = SCR_HEIGHT / 2;
//// Timing
//static float deltaTime = 0.0f;
//static float lastFrame = 0.0f;
//
//SphShow::SphShow(){
//    fluidsystem.init(pointCount, &wallBoxMin, &wallBoxMax, &fluidBoxMin, &fluidBoxMax, &gravity);
//}
//
//GLFWwindow* SphShow::initWindow(){
//    glfwInit();
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//    #ifdef __APPLE__
//    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//    #endif
//
//    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "sph", NULL, NULL);
//    glfwMakeContextCurrent(window);
//    if (window == NULL)
//    {
//        std::cout << "Failed to create GLFW window" << std::endl;
//        glfwTerminate();
//    }
////    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//    {
//        std::cout << "Failed to initialize GLAD" << std::endl;
//    }
//    glfwSetScrollCallback(window, scroll_callback);
//    glfwSetCursorPosCallback(window, mouse_callback);
//
//    glEnable(GL_DEPTH_TEST);
//    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
//    return window;
//}
//
//void SphShow::render(){
//    GLFWwindow* window = this->initWindow();
//    Shader shader(sphVs, sphFs);
//    Shader sphPointShader(sphPointVs, sphPointFs);
//    while (!glfwWindowShouldClose(window)) {
//        float currentFrame = glfwGetTime();
//        deltaTime = currentFrame - lastFrame;
//        lastFrame = currentFrame;
//        glClearColor(0.0, 0.0, 0.0, 1.0);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//        processInput(window);
//        fluidsystem.tick();
//        glm::mat4 model = glm::mat4(1.0);
//        glm::mat4 view = camera.GetViewMatrix();
//        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
//        shader.use();
//        glLineWidth(3);
//        shader.setMat4("model", model);
//        shader.setMat4("view", view);
//        shader.setMat4("projection", projection);
//        this->setBoxData();
//        sphPointShader.use();
////        model = glm::scale(model, glm::vec3(1.5, 1.5, 1.5));
//        glPointSize(3);
//        sphPointShader.setMat4("model", model);
//        sphPointShader.setMat4("view", view);
//        sphPointShader.setMat4("projection", projection);
//        this->setPointData();
//        
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//    
//    glfwTerminate();
//}
//
//void SphShow::setPointData(){
//    std::vector<float> pointPositionData = fluidsystem.getPointPosition();
//    float pointPositionDataArray[pointPositionData.size()];
//    for(unsigned int i = 0; i < pointPositionData.size(); ++i){
//        pointPositionDataArray[i] = pointPositionData[i];
//    }
//
//    if(pointVao == 0){
//        glGenVertexArrays(1, &pointVao);
//        glGenBuffers(1, &pointVbo);
//        glBindBuffer(GL_ARRAY_BUFFER, pointVbo);
//        glBufferData(GL_ARRAY_BUFFER, sizeof(pointPositionDataArray), pointPositionDataArray, GL_STATIC_DRAW);
//        glBindVertexArray(pointVao);
//        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//        glEnableVertexAttribArray(0);
//        glBindVertexArray(0);
//    }
//    glBindBuffer(GL_ARRAY_BUFFER, pointVbo);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(pointPositionDataArray), pointPositionDataArray, GL_STATIC_DRAW);
//    glBindVertexArray(pointVao);
//    unsigned int drawCount = fluidsystem.getPointCounts();
//    glDrawArrays(GL_POINTS, 0, drawCount);
//    
//}
//
//void SphShow::setBoxData(){
//    float xMin = wallBoxMin.x;
//    float yMin = wallBoxMin.y;
//    float zMin = wallBoxMin.z;
//    float xMax = wallBoxMax.x;
//    float yMax = wallBoxMax.y;
//    float zMax = wallBoxMax.z;
//    float vertices[] = {
//        xMin, yMin, zMin,
//        xMax, yMin, zMin,
//        
//        xMax, yMin, zMin,
//        xMax, yMax, zMin,
//        
//        xMax, yMax, zMin,
//        xMin, yMax, zMin,
//        
//        xMin, yMax, zMin,
//        xMin, yMin, zMin,
//        
//        xMin, yMin, zMin,
//        xMin, yMin, zMax,
//        
//        xMin, yMin, zMax,
//        xMin, yMax, zMax,
//        
//        xMin, yMax, zMax,
//        xMin, yMax, zMin,
//        
//        xMin, yMin, zMax,
//        xMax, yMin, zMax,
//        
//        xMin, yMax, zMax,
//        xMax, yMax, zMax,
//        
//        xMax, yMax, zMax,
//        xMax, yMin, zMax,
//        
//        xMax, yMax, zMax,
//        xMax, yMax, zMin,
//        
//        xMax, yMin, zMax,
//        xMax, yMin, zMin
//    };
//    if(boxVao == 0){
//        //创建顶点数组对象VAO
//        glGenVertexArrays(1, &boxVao);
//        glBindVertexArray(boxVao);
//
//        //一个缓冲ID生成一个VBO对象
//        glGenBuffers(1, &boxVbo);
//        //绑定一个顶点缓冲对象绑定到GL_ARRAY_BUFFER
//        glBindBuffer(GL_ARRAY_BUFFER, boxVbo);
//        //使用的任何（在GL_ARRAY_BUFFER目标上的）缓冲调用都会用来配置当前绑定的缓冲(VBO)。然后我们可以调用glBufferData函数，它会把之前定义的顶点数据复制到缓冲的内存中
//        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//        glEnableVertexAttribArray(0);
//        glBindVertexArray(0);
//    }
//    glBindVertexArray(boxVao);
//    glDrawArrays(GL_LINES, 0, 24);
//    glBindVertexArray(0);
//}
//
//static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
//    camera.ProcessMouseScroll(yoffset);
//}
//
//static void mouse_callback(GLFWwindow *window, double xpos, double ypos){
//    if (firstMouse)
//    {
//        lastX = xpos;
//        lastY = ypos;
//        firstMouse = false;
//    }
//
//    float xoffset = xpos - lastX;
//    float yoffset = lastY - ypos;
//
//    lastX = xpos;
//    lastY = ypos;
//
//    camera.ProcessMouseMovement(xoffset, yoffset);
//}
//
//static void processInput(GLFWwindow *window){
//    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
//        glfwSetWindowShouldClose(window, true);
//    }
//    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
//        camera.ProcessKeyboard(FORWARD, deltaTime);
//    }
//    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
//        camera.ProcessKeyboard(BACKWARD, deltaTime);
//    }
//    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
//        camera.ProcessKeyboard(LEFT, deltaTime);
//    }
//    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
//        camera.ProcessKeyboard(RIGHT, deltaTime);
//    }
//    else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
//        camera.ProcessKeyboard(TOP, deltaTime);
//    }
//    else if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
//        camera.ProcessKeyboard(DOWN, deltaTime);
//    }
//}
