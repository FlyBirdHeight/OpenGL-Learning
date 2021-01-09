#ifndef text_h
#define text_h
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>
#include <map>
#include <utility>
#include <locale>
#include <ft2build.h>
#include FT_FREETYPE_H


#define STB_IMAGE_IMPLEMENTATION    // include之前必须定义
#include "stb_image.h"
#include "camera.h"
#include "shaders.h"
#pragma execution_character_set("utf-8")

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

struct Character {
    //字形纹理ID
    unsigned int TextureID;
    //字形大小,width,height
    glm::ivec2 Size;
    //从基准线到字形左部/顶部的偏移值
    glm::ivec2 Bearing;
    //原点到下一个字形原点的距离
    unsigned int Advance;
};
std::map<char, Character> Characters;

class Text {
public:
    unsigned int textVao, textVbo;
    void initTextShow() {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
        #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        #endif
        GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "文本渲染", NULL, NULL);
        glfwMakeContextCurrent(window);
//        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        if (window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
        }

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
        }
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        setlocale(LC_ALL, "chs");
        FT_Library ft;
        if (FT_Init_FreeType(&ft)) {
            std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        }
        FT_Face face;
        if (FT_New_Face(ft, fontPath, 0, &face)) {
            std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        }
        else {
            //宽度值设为0表示我们要从字体面通过给定的高度中动态计算出字形的宽度
            FT_Set_Pixel_Sizes(face, 0, 48);
            //字节对齐
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            for (unsigned char c = 0; c < 128; c++) {
                if (FT_Load_Char(face, c, FT_LOAD_RENDER))
                {
                    std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                    continue;
                }
                // generate texture
                unsigned int texture;
                glGenTextures(1, &texture);
                glBindTexture(GL_TEXTURE_2D, texture);
                glTexImage2D(
                    GL_TEXTURE_2D,
                    0,
                    GL_RED,
                    face->glyph->bitmap.width,
                    face->glyph->bitmap.rows,
                    0,
                    GL_RED,
                    GL_UNSIGNED_BYTE,
                    face->glyph->bitmap.buffer
                );
                // set texture options
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                // now store character for later use
                Character character = {
                    texture,
                    glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                    glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                    static_cast<unsigned int>(face->glyph->advance.x)
                };
                Characters.insert(std::pair<char, Character>(c, character));
            }
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        
        FT_Done_Face(face);
        FT_Done_FreeType(ft);
        
       
        glGenVertexArrays(1, &textVao);
        glGenBuffers(1, &textVbo);
        glBindVertexArray(textVao);
        glBindBuffer(GL_ARRAY_BUFFER, textVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        
        Shader shader(textVs, textFs);
        glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
        shader.use();
        shader.setInt("text", 0);
        shader.setMat4("projection", projection);
        while (!glfwWindowShouldClose(window))
        {
            processInput(window);
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            wchar_t school[] = L"浙江工业大学";
            wchar_t name[] = L"李景秋";
            RenderChinaText(shader, school, 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
            RenderChinaText(shader, name, 540.0f, 540.0f, 1.0f, glm::vec3(0.3, 0.7f, 0.9f));

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
        glfwTerminate();
    }
private:
    const char* fontPath = "./resources/font/default.ttf";
    std::string textVs = "./shader/vs/combat/text.vs";
    std::string textFs = "./shader/fs/combat/text.fs";
    void processInput(GLFWwindow *window)
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    }
    void RenderChinaText(Shader &shader, wchar_t text[], float x, float y, float scale, glm::vec3 color) {
        shader.use();
        shader.setVec3("textColor", color);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(textVao);

        for (int i = 0; i < sizeof(text); i++) {
            Character ch = Characters[text[i]];
            float xpos = ch.Size.x * scale + x;
            float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

            float w = ch.Size.x * scale;
            float h = ch.Size.y * scale;
            // 对每个字符更新VBO
            float vertices[6][4] = {
                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos,     ypos,       0.0f, 1.0f },
                { xpos + w, ypos,       1.0f, 1.0f },

                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos + w, ypos,       1.0f, 1.0f },
                { xpos + w, ypos + h,   1.0f, 0.0f }
            };
            // 在四边形上绘制字形纹理
            glBindTexture(GL_TEXTURE_2D, ch.TextureID);
            // 更新VBO内存的内容
            glBindBuffer(GL_ARRAY_BUFFER, textVbo);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            // 绘制四边形
            glDrawArrays(GL_TRIANGLES, 0, 6);
            // 更新位置到下一个字形的原点，注意单位是1/64像素
            x += (ch.Advance >> 6) * scale; // 位偏移6个单位来获取单位为像素的值 (2^6 = 64)
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    void RenderText(Shader &shader, std::string text, float x, float y, float scale, glm::vec3 color) {
        shader.use();
        shader.setVec3("textColor" , color);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(textVao);
        
        for (int i = 0; i < text.length(); i++) {
            Character ch = Characters[text[i]];
            float xpos = ch.Size.x * scale + x;
            float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

            float w = ch.Size.x * scale;
            float h = ch.Size.y * scale;
            // 对每个字符更新VBO
            float vertices[6][4] = {
                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos,     ypos,       0.0f, 1.0f },
                { xpos + w, ypos,       1.0f, 1.0f },

                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos + w, ypos,       1.0f, 1.0f },
                { xpos + w, ypos + h,   1.0f, 0.0f }
            };
            // 在四边形上绘制字形纹理
            glBindTexture(GL_TEXTURE_2D, ch.TextureID);
            // 更新VBO内存的内容
            glBindBuffer(GL_ARRAY_BUFFER, textVbo);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            // 绘制四边形
            glDrawArrays(GL_TRIANGLES, 0, 6);
            // 更新位置到下一个字形的原点，注意单位是1/64像素
            x += (ch.Advance >> 6) * scale; // 位偏移6个单位来获取单位为像素的值 (2^6 = 64)
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
};



#endif /* text_h */
