////#define STB_IMAGE_IMPLEMENTATION    // include之前必须定义
////#include "stb_image.h"
//#include "terrain.hpp"
//#define TINYOBJLOADER_IMPLEMENTATION
//#include <tiny_obj_loader.h>
//#include <fstream>
//void processInput(GLFWwindow *window, Shader &shader);
//void mouse_callback(GLFWwindow *window, double xpos, double ypos);
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//
////窗口大小
//const unsigned int SCR_HEIGHT = 1080, SCR_WIDTH = 1920;
////相机类
//Camera camera;
////是否第一次点击
//bool firstMouse = true;
//float lastX = SCR_WIDTH / 2;
//float lastY = SCR_HEIGHT / 2;
//// Timing
//float deltaTime = 0.0f;
//float lastFrame = 0.0f;
//float currentFrame;
///**
// 说明一下，为什么这里都是用float存数据的问题
// 因为我们在使用opengl去绑定着色器的时候，我们需要确定数据的步长，如果使用glm::vec3的话，就不能很好地让opengl去理解，也没办使用顶点缓冲对象、顶点数组对象和顶点索引对象了
// 所以，我们这里全部都要使用vector<float>的形式，去存储顶点坐标、颜色、法线、三角形顶点索引数据
// */
//Terrain::Terrain(){
//    camera.Position = glm::vec3(this->originX, 20.0, this->originY);
//}
//
////窗口创建
//void Terrain::createWindow(){
//    glfwInit();
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//    #ifdef __APPLE__
//    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//    #endif
//
//    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Generate Terrain", NULL, NULL);
//    glfwMakeContextCurrent(window);
////    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//    if (window == NULL)
//    {
//        std::cout << "Failed to create GLFW window" << std::endl;
//        glfwTerminate();
//    }
//
//    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//    {
//        std::cout << "Failed to initialize GLAD" << std::endl;
//    }
//    glfwSetScrollCallback(window, scroll_callback);
//    glfwSetCursorPosCallback(window, mouse_callback);
//
//    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_FRAMEBUFFER_SRGB);
//    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
//    this->init(window);
//}
////初始化数据并执行渲染操作
//void Terrain::init(GLFWwindow* window){
//    std::vector<unsigned int> mapChunkVao(this->xMapChunk * this->yMapChunk);
//    //三角形顶点索引数组
//    std::vector<int> indices = this->generate_indice();
//    //地形附着植物数据
//    std::vector<plant> plants;
//    for(unsigned int y = 0; y < this->yMapChunk; ++y){
//        for(unsigned int x = 0; x < this->xMapChunk; ++x){
//            //生成噪声值
//            std::vector<float> noiseMap;
//            noiseMap = this->generate_noise(x, y);
//            //生成每个网格中的坐标点
//            std::vector<float> vertices = this->generate_vertices(noiseMap, x, y);
//            //生成地形颜色群落
//            std::vector<float> returnColor = this->generate_biome(vertices, plants, x, y);
//            //生成法线数据
//            std::vector<float> normals = this->generate_normal(indices, vertices);
//            //生成地图模型
//            unsigned int mapVao = x + y * this->yMapChunk;
//            this->generate_map_chunk(mapChunkVao[mapVao], vertices, indices, returnColor, normals);
//        }
//    }
//
//    //生成植物数据
//    std::vector<unsigned int> treeVAO(xMapChunk * yMapChunk);
//      std::vector<unsigned int> flowerVAO(xMapChunk * yMapChunk);
//    set_model(treeVAO, "tree", plants, "./resources/obj/CommonTree_1.obj", "./resources/obj");
//    set_model(flowerVAO, "flower", plants, "./resources/obj/Flowers.obj", "./resources/obj");
//    //渲染设置
//    this->render(window, mapChunkVao, treeVAO, flowerVAO);
//}
//
///**
// 渲染设置
// */
//void Terrain::render(GLFWwindow* window, std::vector<unsigned int> mapChunkVao, std::vector<unsigned int> treeVao, std::vector<unsigned int> flowerVao){
//    Shader shader("./shader/vs/terrain/terrain.vs","./shader/fs/terrain/terrain.fs");
//    shader.use();
//    shader.setBool("iFrame", true);
//    shader.setVec3("light.ambient", glm::vec3(0.2, 0.2, 0.2));
//    shader.setVec3("light.diffuse", glm::vec3(0.3, 0.3, 0.3));
//    shader.setVec3("light.specular", glm::vec3(1.0, 1.0, 1.0));
//    shader.setVec3("light.direction", glm::vec3(-0.2, -1.0, -0.3));
//
//    while (!glfwWindowShouldClose(window)) {
//        currentFrame = glfwGetTime();
//        deltaTime = currentFrame - lastFrame;
//        lastFrame = currentFrame;
//
//        processInput(window, shader);
//
//        glClearColor(0.1, 0.1, 0.1, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//
//        shader.use();
//        shader.setMat4("view", camera.GetViewMatrix());
//        shader.setMat4("projection", glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, (float)this->chunkWidth * (this->chunk_render_distance - 1.2f)));
//        shader.setVec3("viewPos", camera.Position);
//        glm::mat4 model(1.0f);
//        //这个就是之后要画三角形面时候，所要用到的顶点个数的计算
//        int nIndices = (this->chunkHeight - 1) * (this->chunkWidth - 1) * 6;
//        //计算在相机之外的大区域的个数
//        gridPosX = (int)(camera.Position.x - originX) / this->chunkWidth + xMapChunk / 2;
//        gridPosY = (int)(camera.Position.z - originY) / this->chunkHeight + yMapChunk/ 2;
//        for(int y = 0; y < this->yMapChunk; ++y){
//            for(int x = 0; x < this->xMapChunk; ++x){
//                //这里渲染的时候不是全部渲染，需要渲染的只是在3*3范围内的大区域，节省时间，所以需要计算距离位置
////                if(std::abs(gridPosX - x) <= chunk_render_distance && (gridPosY - y) <= chunk_render_distance){
////
////                }
//                shader.use();
//                model = glm::mat4(1.0f);
//                //计算当前需要渲染所在区域的位置的中心,并赋值给model进行偏移。
//                model = glm::translate(model, glm::vec3(-this->chunkWidth / 2.0 + (this->chunkWidth - 1) * x, 0.0, -this->chunkHeight / 2.0 + (this->chunkHeight - 1) * y));
//                shader.setMat4("model", model);
//
//                glBindVertexArray(mapChunkVao[x + y * this->yMapChunk]);
//                glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);
//
//                //绘制区域内的植物分布
//                model = glm::mat4(1.0f);
//                model = glm::translate(model, glm::vec3(-this->chunkWidth / 2.0 + (this->chunkWidth - 1) * x, 0.0, -this->chunkHeight / 2.0 + (this->chunkHeight - 1) * y));
//                //这里需要对植物的大小进行一定的缩放
//                model = glm::scale(model, glm::vec3(this->MODEL_SCALE));
//                //开启面剔除
//                glEnable(GL_CULL_FACE);
//                glBindVertexArray(flowerVao[x + y * this->yMapChunk]);
//                glDrawArraysInstanced(GL_TRIANGLES, 0, 1300, 16);
//
//                glBindVertexArray(treeVao[x + y * this->yMapChunk]);
//                glDrawArraysInstanced(GL_TRIANGLES, 0, 10192, 8);
//                glDisable(GL_CULL_FACE);
//            }
//        }
//
//        glfwPollEvents();
//        glfwSwapBuffers(window);
//    }
//
//    glfwTerminate();
//}
///**
// 生成地图模型
// @param {std::vector<unsigned int>} mapChunkVao 所在的顶点数组对象的地区（按大区域划分），主要作用是用来按需渲染
// @param {std::vector<float>} vertices 网格顶点坐标
// @param {std::vector<int>} indices 三角形平面顶点索引
// @param {std::vector<float>} colorCard 群落颜色
// @param {std::vector<float>} normals 三角形平面法线
// */
//void Terrain::generate_map_chunk(unsigned int &mapChunkVao, std::vector<float> vertices, std::vector<int> indices, std::vector<float> colorCard, std::vector<float> normals){
//    unsigned int verticeVBO, normalVBO, colorCardVBO;
//    unsigned int EBO;
//    glGenBuffers(1, &verticeVBO);
//    glGenBuffers(1, &EBO);
//    glGenVertexArrays(1, &mapChunkVao);
//    glBindVertexArray(mapChunkVao);
//    //顶点数据绑定
//    glBindBuffer(GL_ARRAY_BUFFER, verticeVBO);
//    glBufferData(GL_ARRAY_BUFFER, vertices.size(), &vertices[0], GL_STATIC_DRAW);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size(), &indices[0], GL_STATIC_DRAW);
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//    //法线数据绑定
//    glGenBuffers(1, &normalVBO);
//    glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
//    glBufferData(GL_ARRAY_BUFFER, normals.size(), &normals[0], GL_STATIC_DRAW);
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(1);
//    //颜色数据绑定
//    glGenBuffers(1, &colorCardVBO);
//    glBindBuffer(GL_ARRAY_BUFFER, colorCardVBO);
//    glBufferData(GL_ARRAY_BUFFER, colorCard.size(), &colorCard[0], GL_STATIC_DRAW);
//    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(2);
//}
//
//
//
///**
// 生成构成平面的三角形顶点，主要用于顶点索引对象使用
// */
//std::vector<int> Terrain::generate_indice(){
//    std::vector<int> returnData;
//    for(unsigned int y = 0; y < this->chunkHeight; y++){
//        for(unsigned int x  = 0; x < this->chunkWidth; x++){
//            int pos = x + y * this->chunkWidth;
//            if(x == this->chunkWidth - 1 || y == this->chunkHeight - 1){
//                continue;
//            }else{
//                returnData.push_back(pos + this->chunkWidth);
//                returnData.push_back(pos);
//                returnData.push_back(pos + 1 + this->chunkWidth);
//                returnData.push_back(pos + 1);
//                returnData.push_back(pos + 1+ this->chunkWidth);
//                returnData.push_back(pos);
//            }
//        }
//    }
//
//    return returnData;
//}
///**
// 对每一个网格生成相对应的噪声值，10 * 10的大网格内部的127 * 127的小网格
// @param {int} xOffset 横向大区域偏移量
// @param {int} yOffset z方向大区域偏移量
// */
//std::vector<float> Terrain::generate_noise(int xOffset, int yOffset){
//    std::vector<float> noiseValues;
//
//    for(unsigned int y = 0; y < this->chunkHeight; ++y){
//        for(unsigned int x = 0; x < this->chunkWidth; x++){
//            float noiseData = this->perlinNoise.fbm((x + xOffset * (chunkWidth - 1)) / this->noiseScale, (y + yOffset * (chunkHeight - 1)) / this->noiseScale);
//            noiseValues.push_back(noiseData);
//        }
//    }
//
//    return noiseValues;
//}
///**
// 生成网格数据
// @param {const std::vector<float>} noise_map 噪声数据值
// @param {int} xOffset 横向大区域偏移量
// @param {int} yOffset z方向大区域偏移量
// */
//std::vector<float> Terrain::generate_vertices(const std::vector<float> &noise_map, int xOffset, int yOffset){
//    std::vector<float> vertices;
//    for(unsigned int y = 0; y < this->chunkHeight; ++y){
//        for(unsigned int x = 0; x < this->chunkWidth; ++x){
//            vertices.push_back(x);
//            //这里实际上要对生成的噪点值进行缩放，来保证不同高度生成
//            float scaleNoise = std::fmax(std::pow(noise_map[x + y * this->chunkHeight] * 1.1, 3) * this->meshHeight, this->horizontal * 0.5 * this->meshHeight);
//            vertices.push_back(scaleNoise);
//            vertices.push_back(y);
//        }
//    }
//
//    return vertices;
//}
///**
// 生成地形群落
// @param {std::vector<float>} vertices 网格顶点坐标集合
// @param {std::vector<int>} plant 植物集合
// @param {int} xOffset 横向大区域偏移量
// @param {int} yOffset z方向大区域偏移量
// @return {std::vector<float>} 颜色数据组
// */
//std::vector<float> Terrain::generate_biome(const std::vector<float> &vertices, std::vector<plant> &plants, int xOffset, int yOffset){
//    std::vector<float> colors;
//    std::vector<terrainColor> biomeColors;
//    glm::vec3 color = this->handleColor(255, 255, 255);
//    //设置颜色，根据不同的高度设置
//    biomeColors.push_back(terrainColor(this->horizontal * 0.5, this->handleColor(60,  95, 190)));   // 深水
//    biomeColors.push_back(terrainColor(this->horizontal, this->handleColor(60, 100, 190)));  // 浅水
//    biomeColors.push_back(terrainColor(0.15, this->handleColor(210, 215, 130)));                // 陆地
//    biomeColors.push_back(terrainColor(0.30, this->handleColor( 95, 165,  30)));                // 草地1
//    biomeColors.push_back(terrainColor(0.40, this->handleColor( 65, 115,  20)));                // 草地2
//    biomeColors.push_back(terrainColor(0.50, this->handleColor( 90,  65,  60)));                // Rock 1
//    biomeColors.push_back(terrainColor(0.80, this->handleColor( 75,  60,  55)));                // Rock 2
//    biomeColors.push_back(terrainColor(1.00, this->handleColor(255, 255, 255)));                // 积雪
//    std::string plantType;
//    //老样子，遍历一下顶点坐标，然后来分配不同高度的着色, 注意这里是要高度，所以是要用Y的坐标，所以从1的位置开始，每次跳跃3个高度
//    for(int i = 1; i < vertices.size(); i += 3){
//        for(int j = 0; j < biomeColors.size(); j++){
//            if(vertices[i] <= biomeColors[j].height * this->meshHeight){
//                color = biomeColors[j].color;
//                //这里是用来判断是否需要放置植被的，可以进行自定义，随意都可以，但是需要在陆地以上
//                if(j == 3){
//                    if(rand() % 1000 < 5){
//                        if(rand() % 100 < 70){
//                            plantType = "flower";
//                        }else{
//                            plantType = "tree";
//                        }
//                        plants.push_back(plant(plantType, vertices[i-1], vertices[i], vertices[i + 1], xOffset, yOffset));
//                    }
//                }
//                break;
//            }
//        }
//        colors.push_back(color.x);
//        colors.push_back(color.y);
//        colors.push_back(color.z);
//    }
//
//
//    return colors;
//}
///**
// 生成法向量
// @param {std::vector<int>} indices 三角形平面索引数据
// @param {std::vector<float>} vertices 网格顶点数据
// */
//std::vector<float> Terrain::generate_normal(const std::vector<int> &indices, const std::vector<float> &vertices){
//    std::vector<float> normals;
//    glm::vec3 normal;
//    //这里计算每个三角形平面的法向量，所以要根据顶点索引数组进行坐标点的取值，然后进行向量叉乘，获得法向量，同时由于这里使用的是右手螺旋，所以最后要记得取反
//    for(int i= 0; i < indices.size(); i += 3){
//        std::vector<glm::vec3> vert;
//        for(int j = 0; j < 3; j++){
//            //因为存储坐标的3个为一个步长，所以这里乘以3，而且我们需要三角形三个顶点的坐标，所以这里要循环三次
//            int pos = indices[i + j] * 3;
//            vert.push_back(glm::vec3(vertices[pos], vertices[pos + 1], vertices[pos + 2]));
//        }
//        glm::vec3 vectorA = vert[1] - vert[0];
//        glm::vec3 vectorB = vert[2] - vert[0];
//        glm::vec3 normal = glm::normalize(-glm::cross(vectorA, vectorB));
//        normals.push_back(normal.x);
//        normals.push_back(normal.y);
//        normals.push_back(normal.z);
//    }
//
//    return normals;
//}
///**
// 处理模型数据
// */
//void Terrain::load_model(unsigned int &VAO, std::string filename, std::string materialName){
//    //索引
//    std::vector<float> vertices;
//    //顶点坐标
//    std::vector<int> indices;
//    //形状
//    std::vector<tinyobj::shape_t> shapes;
//    //材质（也就是rgb颜色）
//    std::vector<tinyobj::material_t> materials;
//    tinyobj::attrib_t attrib;
//    std::string warning;
//    std::string error;
//    bool status = tinyobj::LoadObj(&attrib, &shapes, &materials, &warning, &error, filename.c_str(), materialName.c_str());
//    if(!warning.empty()){
//        std::cout<< warning.c_str() << std::endl;
//    }else if(!error.empty()){
//        std::cout << error.c_str() << std::endl;
//    }
//    if(!status){
//        exit(1);
//    }
//    //读取数据
//    for(unsigned int i = 0; i < shapes.size(); ++i){
//        int index_offset = 0;
//        for(int j = 0; j < shapes[i].mesh.num_face_vertices.size(); ++j){
//            int fv = shapes[i].mesh.num_face_vertices[j];
//            for(int k = 0; k < fv; ++k){
//                tinyobj::index_t idx = shapes[i].mesh.indices[index_offset + k];
//                //顶点
//                vertices.push_back(attrib.vertices[3 * idx.vertex_index + 0]);
//                vertices.push_back(attrib.vertices[3 * idx.vertex_index + 1]);
//                vertices.push_back(attrib.vertices[3 * idx.vertex_index + 2]);
//                //法线
//                vertices.push_back(attrib.normals[3 * idx.normal_index + 0]);
//                vertices.push_back(attrib.normals[3 * idx.normal_index + 1]);
//                vertices.push_back(attrib.normals[3 * idx.normal_index + 2]);
//                //材质
//                vertices.push_back(materials[shapes[i].mesh.material_ids[j]].diffuse[0] * MODEL_BRIGHTNESS);
//                vertices.push_back(materials[shapes[i].mesh.material_ids[j]].diffuse[1] * MODEL_BRIGHTNESS);
//                vertices.push_back(materials[shapes[i].mesh.material_ids[j]].diffuse[2] * MODEL_BRIGHTNESS);
//            }
//            index_offset += fv;
//        }
//    }
//
//    unsigned int VBO;
//    glGenBuffers(1, &VBO);
//    glGenVertexArrays(1, &VAO);
//    glBindVertexArray(VAO);
//    glBindBuffer(GL_ARRAY_BUFFER,VBO);
//    glBufferData(GL_ARRAY_BUFFER, vertices.size(), &vertices[0], GL_STATIC_DRAW);
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
//    glEnableVertexAttribArray(1);
//    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
//    glEnableVertexAttribArray(2);
//
//}
///**
// 设置模型偏移位置
// */
//void Terrain::set_model(std::vector<unsigned int> &plant_chunk, std::string type, std::vector<plant> &plants, std::string filename, std::string materialName){
//    std::vector<std::vector<float>> chunkInstances;
//    chunkInstances.resize(xMapChunk * yMapChunk);
//
//    for(int i = 0; i < plants.size(); ++i){
//        float xPos = plants[i].xpos / MODEL_SCALE;
//        float yPos = plants[i].ypos / MODEL_SCALE;
//        float zPos = plants[i].zpos / MODEL_SCALE;
//        //这个pos实际就是创建的chunkInstances中的下标位置，方向计算是先从左到右，在从上到下
//        int pos = plants[i].xOffset + plants[i].yOffset * xMapChunk;
//        if(plants[i].type == type){
//            chunkInstances[pos].push_back(xPos);
//            chunkInstances[pos].push_back(yPos);
//            chunkInstances[pos].push_back(zPos);
//        }
//    }
//    //这里的顶点缓冲对象都是按照每一个大区域进行渲染的，也就是10 * 10, 100个顶点缓冲对象这样处理的
//    unsigned int instancesVBO[xMapChunk * yMapChunk];
//    for(int y = 0; y < yMapChunk; ++y){
//        for(int x = 0; x < xMapChunk; ++x){
//            int pos = x + y * xMapChunk;
//            this->load_model(plant_chunk[pos], filename, materialName);
//
//            glBindVertexArray(plant_chunk[pos]);
//            glBindBuffer(GL_ARRAY_BUFFER, instancesVBO[pos]);
//            glBufferData(GL_ARRAY_BUFFER, chunkInstances[pos].size(), &chunkInstances[pos][0], GL_STATIC_DRAW);
//            glEnableVertexAttribArray(3);
//            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//            glVertexAttribDivisor(3, 1);
//        }
//    }
//}
//
///**
// 处理颜色数据
// */
//glm::vec3 Terrain::handleColor(int r, int g, int b){
//    return glm::vec3(r / 255.0, g / 255.0, b / 255.0);
//}
//
//
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
//    camera.ProcessMouseScroll(yoffset);
//}
//
//void mouse_callback(GLFWwindow *window, double xpos, double ypos){
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
//void processInput(GLFWwindow *window, Shader &shader){
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
//
//
//void Terrain::initTestData(){
//    std::vector<unsigned int> mapChunkVao(this->xMapChunk * this->yMapChunk);
//    //三角形顶点索引数组
//    std::vector<int> indices = this->generate_indice();
//    //地形顶点数据集合(测试用)
//    std::vector<float> verticesData;
//    //地形着色数据(测试用)
//    std::vector<float> colorCard;
//    //法线总集合(测试用)
//    std::vector<float> normalsData;
//    //地形附着植物数据
//    std::vector<plant> plants;
//    for(unsigned int y = 0; y < this->yMapChunk; ++y){
//        for(unsigned int x = 0; x < this->xMapChunk; ++x){
//            //生成噪声值
//            std::vector<float> noiseMap;
//            noiseMap = this->generate_noise(x, y);
//            //生成每个网格中的坐标点
//            std::vector<float> vertices = this->generate_vertices(noiseMap, x, y);
//            verticesData.insert(verticesData.end(), vertices.begin(), vertices.end());
//            //生成地形颜色群落
//            std::vector<float> returnColor = this->generate_biome(vertices, plants, x, y);
//            colorCard.insert(colorCard.end(), returnColor.begin(), returnColor.end());
//            //生成法线数据
//            std::vector<float> normals = this->generate_normal(indices, vertices);
//            normalsData.insert(normalsData.end(), normals.begin(), normals.end());
//        }
//    }
//    //数据测试
//    this->test(verticesData, indices, colorCard, normalsData);
//}
////数据测试
//void Terrain::test(std::vector<float> vertices, std::vector<int> indices, std::vector<float> colorCard, std::vector<float> normals){
//    std::ofstream outFile("./resources/data_write/mesh.txt");
//    outFile << "顶点三角形平面索引" << std::endl;
//    for(unsigned int i = 0; i < indices.size(); i += 3){
//        outFile << indices[i] << "   " << indices[i+1] << "   " << indices[i+2] << std::endl;
//    }
//    outFile << std::endl;
//    outFile << "顶点坐标" << std::endl;
//    for(unsigned int i = 0; i < vertices.size(); i += 3){
//        outFile << vertices[i] << "   " << vertices[i+1] << "   " << vertices[i+2] << std::endl;
//    }
////    outFile << std::endl;
////    outFile << "颜色数据" << std::endl;
////    for(unsigned int i = 0; i < colorCard.size(); i += 3){
////        outFile << colorCard[i] << "   " << colorCard[i+1] << "   " << colorCard[i+2] << std::endl;
////    }
////    outFile << std::endl;
////    outFile << "法线数据" << std::endl;
////    for(unsigned int i = 0; i < normals.size(); i += 3){
////        outFile << normals[i] << "   " << normals[i+1] << "   " << normals[i+2] << std::endl;
////    }
////
//    std::cout << "写出成功" << std::endl;
//}
