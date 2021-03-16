#include "generate_mesh.hpp"
void process(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
Camera camera;
const unsigned int SCR_WIDTH = 1920, SCR_HEIGHT = 1080;
int xMapChunk = 1;
int yMapChunk = 1;
int chunkWidth = 127;
int chunkHeight = 127;
int chunk_render_distance = 3;
//噪声相关参数设置
int octaves = 5;//度数
float meshHeight = 32;//顶点缩放
float noiseScale = 64;//水平缩放
float horizontal = 0.5;
//是否第一次点击
bool firstMouse = true;
float lastX = SCR_WIDTH / 2;
float lastY = SCR_HEIGHT / 2;
// Timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float currentFrame;
float originX = (chunkWidth  * xMapChunk) / 2 - chunkWidth / 2;
float originY = (chunkHeight * yMapChunk) / 2 - chunkHeight / 2;
GenerateMesh::GenerateMesh(){
    camera.Position = glm::vec3(0.0, 0.0, 3.0);
}

void GenerateMesh::initTest(){
    std::vector<unsigned int> indices = this->generate_indices();
    std::vector<float> vertices;
    for(int y = 0; y < yMapChunk; ++y){
        for(int x = 0; x < xMapChunk; ++x){
            std::vector<float> noise = this->generate_noises(x, y);
            std::vector<float> vertice= this->generate_vertices(x, y, noise);
            vertices.insert(vertices.end(), vertice.begin(), vertice.end());
        }
    }
    std::ofstream outFile("./resources/data_write/mesh.txt");
    outFile << "顶点三角形平面索引" << std::endl;
    for(unsigned int i = 0; i < indices.size(); i += 3){
        outFile << indices[i] << "," << indices[i+1] << "," << indices[i+2] << "," <<std::endl;
    }
    outFile << std::endl;
    outFile << "顶点坐标" << std::endl;
    for(unsigned int i = 0; i < vertices.size(); i += 3){
        outFile << vertices[i] << ".0," << vertices[i+1] << ".0," << vertices[i+2]<< ".0," << std::endl;
    }
    
    std::cout << "success" << std::endl;
}

void GenerateMesh::createWindow(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Generate Terrain", NULL, NULL);
    glfwMakeContextCurrent(window);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_FRAMEBUFFER_SRGB);
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    this->init(window);
}
void GenerateMesh::init(GLFWwindow *window){
    
    std::vector<unsigned int> mapVao;
    mapVao.resize(xMapChunk * yMapChunk);
    unsigned int count = 0;
    for(int y = 0; y < yMapChunk; ++y){
        for(int x = 0; x < xMapChunk; ++x){
            std::vector<float> noise = this->generate_noises(x, y);
            std::vector<float> vertice= this->generate_vertices(x, y, noise);
            count = x + y * yMapChunk;
            this->generateMap(mapVao[count], vertice);
        }
    }
    this->render(window, mapVao);
}

void GenerateMesh::generateMap(unsigned int &mapVao, std::vector<float> vertices){
    std::vector<unsigned int> indices = this->generate_indices();
    unsigned int vbo, ebo;
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glGenVertexArrays(1, &mapVao);
    glBindVertexArray(mapVao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size(), &vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size(), &indices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBindVertexArray(0);
}

void GenerateMesh::render(GLFWwindow *window, std::vector<unsigned int> mapChunkVao){
    Shader shader("./shader/vs/mesh/mesh.vs", "./shader/fs/mesh/mesh.fs");
    while (!glfwWindowShouldClose(window)) {
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        process(window);
        glClearColor(0.0, 0.0, 0.0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        shader.use();
        shader.setMat4("view", camera.GetViewMatrix());
        shader.setMat4("projection", glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f));
        for(int y = 0; y < yMapChunk; ++y){
            for(int x = 0 ; x < xMapChunk; ++x){
                shader.use();
                glm::mat4 model = glm::mat4(1.0f);
                shader.setMat4("model", model);
                glBindVertexArray(mapChunkVao[x + y * yMapChunk]);
                glDrawElements(GL_TRIANGLES, chunkWidth * chunkHeight * 6, GL_UNSIGNED_INT, 0);
            }
        }
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    glfwTerminate();
}


std::vector<unsigned int> GenerateMesh::generate_indices(){
    std::vector<unsigned int> returnData;
    for(unsigned int y = 0; y < chunkHeight; ++y){
        for(unsigned int x = 0; x < chunkWidth; ++x){
            int pos = x + y * chunkHeight;
            //因为上一个区域的最后一列的值是下一个区域的起始，避免重合，所以就不需要去建立索引，最后一行同理
            if(x == chunkWidth - 1 || y == chunkHeight - 1){
                break;
            }else{
                //这里两个三角形我都是用的顺时针进行创建的
                returnData.push_back(pos);
                returnData.push_back(pos + 1 + chunkWidth);
                returnData.push_back(pos + chunkWidth);
                returnData.push_back(pos);
                returnData.push_back(pos + 1);
                returnData.push_back(pos + 1 + chunkWidth);
            }
        }
    }
    return returnData;
}

std::vector<float> GenerateMesh::generate_vertices(int xOffset, int yOffset, std::vector<float> noises){
    std::vector<float> returnData;
    for (unsigned int y = 0; y < chunkHeight; ++y) {
        for (unsigned int x = 0; x < chunkWidth; ++x) {
            returnData.push_back(x + xOffset * (chunkWidth - 1));
            //这里实际上要对生成的噪点值进行缩放，来保证不同高度生成
            float scaleNoise = std::fmax(std::pow(noises[x + y * chunkHeight] * 1.1, 3) * meshHeight, horizontal * 0.5 * meshHeight);
            returnData.push_back(y + yOffset * (chunkHeight - 1));
            returnData.push_back(0.0);
        }
    }
    
    return returnData;
}

std::vector<float> GenerateMesh::generate_noises(int xOffset, int yOffset){
    std::vector<float> returnData;
    for (unsigned int y = 0; y < chunkHeight; ++y) {
        for (unsigned int x = 0; x < chunkWidth; ++x) {
            float noiseData = this->perlin.fbm((x + xOffset * (chunkWidth - 1)) / noiseScale, (y + yOffset * (chunkHeight - 1)) / noiseScale);
            returnData.push_back(noiseData);
        }
    }
    
    return returnData;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(yoffset);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos){
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void process(GLFWwindow *window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
    else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        camera.ProcessKeyboard(TOP, deltaTime);
    }
    else if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        camera.ProcessKeyboard(DOWN, deltaTime);
    }
}

void GenerateMesh::testData(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Generate Terrain123", NULL, NULL);
    glfwMakeContextCurrent(window);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }
//    glfwSetScrollCallback(window, scroll_callback);
//    glfwSetCursorPosCallback(window, mouse_callback);
//    glEnable(GL_DEPTH_TEST);
    
    std::vector<unsigned int> indices = this->generate_indices();
    std::vector<float> noise = this->generate_noises(0, 0);
    std::vector<float> vertices = this->generate_vertices(0, 0, noise);
    Shader shader("./shader/vs/mesh/meshTest.vs", "./shader/fs/mesh/meshTest.fs");
    unsigned int indicesData[indices.size()];
    float verticesData[vertices.size()];
    for(int i = 0; i < indices.size(); ++i){
        indicesData[i] = indices[i];
    }
    for(int i = 0; i < vertices.size(); ++i){
        verticesData[i] = vertices[i];
    }
    shader.use();
    unsigned int vbo,vao,ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesData), verticesData, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesData), indicesData, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    std::cout << vertices.size() << std::endl;
    while (!glfwWindowShouldClose(window)) {
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        process(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        shader.use();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.05));
        model = glm::translate(model, glm::vec3(-20.0, -20.0, 0.0));
        shader.setMat4("model", model);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, sizeof(indicesData), GL_UNSIGNED_INT, 0);
        
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    
    glfwTerminate();
}
