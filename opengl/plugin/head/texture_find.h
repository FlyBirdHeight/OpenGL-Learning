#ifndef texture_find_hpp
#define texture_find_hpp
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <assert.h>
#include <stdio.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype/ftglyph.h>
#include "character_set.h"
#include "shaders.h"
class TextureFind{
public:
    TextureFind(const char* fontFile, int size){
        fontSize = size;
        xStart = 0;
        yStart = 0;
        memset(characterSet, 0, sizeof(characterSet));
        FT_Init_FreeType(&library);
        assert(library != 0);
        
        FT_New_Face(library, fontFile, 0, &face);
        FT_Set_Char_Size(face, fontSize << 6, fontSize << 6, 72, 72);
        assert(face != 0);
    }
    //在纹理中插入字符
    CharacterSet* getCharacter(wchar_t ch){
        //设置字节对齐方式，不对齐
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        if (characterSet[ch].x0 == 0 && characterSet[ch].y0 == 0 && characterSet[ch].x1 == 0 && characterSet[ch].y1 == 0) {
            //判断字体的大小站位是否已经超过了纹理的宽度
            if (xStart + fontSize > 1024) {
                //如果超过了，就将y轴进行平移，之后再将x轴置为0
                xStart = 0;
                yStart += fontSize;
            }
            //这里就是将字形抽出来了，方便后面写
            FT_Load_Glyph(face, FT_Get_Char_Index(face, ch), FT_LOAD_DEFAULT);
            FT_Glyph glyph;
            FT_Get_Glyph(face->glyph, &glyph);
            //这里是把字形的位图抽取出来，方便后面写，不会过长
            FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, 0, 1);
            FT_BitmapGlyph bitmapGlyph = (FT_BitmapGlyph)glyph;
            FT_Bitmap& bitmap = bitmapGlyph->bitmap;
            if(bitmap.width == 0 || bitmap.rows == 0){
                xStart += fontSize / 2;
                
                characterSet[ch].x0       =   xStart;
                characterSet[ch].y0       =   yStart;
                characterSet[ch].x1       =   xStart + bitmap.width;
                characterSet[ch].y1       =   yStart + bitmap.rows;
                characterSet[ch].offsetY  =   0;
                characterSet[ch].offsetX  =   0;
            }else{
                characterSet[ch].x0       =   xStart;
                characterSet[ch].y0       =   yStart;
                characterSet[ch].x1       =   xStart + bitmap.width;
                characterSet[ch].y1       =   yStart + bitmap.rows;
                characterSet[ch].offsetY  =   bitmapGlyph->top;
                characterSet[ch].offsetX  =   bitmapGlyph->left;
                
                unsigned int textureId;
                glGenTextures(1, &textureId);
                glBindTexture(GL_TEXTURE_2D, textureId);
                //这个就是从纹理图上进行采样，指定其相关属性
                glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, bitmap.width, bitmap.rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE, bitmap.buffer);
                // set texture options
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                characterSet[ch].textureId = textureId;
                
                xStart += bitmap.width + 1;
            }
        }
        
        return &characterSet[ch];
    }
    //绘制文字
    void drawText(float x, float y, const wchar_t* text, Shader& shader){
        float texWidth = 1024.0;
        float texHeight = 1024.0;
        float xoffset = x;
        float yoffset = y + 16.0;
        glGenVertexArrays(1, &fontVao);
        glGenBuffers(1, &fontVbo);
        glBindBuffer(GL_ARRAY_BUFFER, fontVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glBindVertexArray(fontVao);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
        glBindVertexArray(0);
        
        shader.use();
        shader.setVec3("textColor", glm::vec3(0.3, 0.7f, 0.9f));
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(fontVao);
        for(unsigned int i = 0; i < wcslen(text); i++){
            CharacterSet* ch = getCharacter(text[i]);
            int h   =   ch->y1 - ch->y0;
            int w   =   ch->x1 - ch->x0;
            float offset  =   (float(h) - float(ch->offsetY));
            float offsetX =   float(ch->offsetX);
            float vertices[6][4];
            
            //bottom-left
            vertices[0][0] = xoffset;
            vertices[0][1] = yoffset - h + offset;
            vertices[0][2] = ch->x1 / texWidth;
            vertices[0][3] = ch->y1 / texHeight;
            //top-right
            vertices[1][0] = xoffset + w;
            vertices[1][1] = yoffset + offset;
            vertices[1][2] = ch->x1 / texWidth;
            vertices[1][3] = ch->y0 / texHeight;
            //bottom-right
            vertices[2][0] = xoffset + w;
            vertices[2][1] = yoffset - h + offset;
            vertices[2][2] = ch->x0 / texWidth;
            vertices[2][3] = ch->y0 / texHeight;
            //top-right
            vertices[3][0] = xoffset + w;
            vertices[3][1] = yoffset + offset;
            vertices[3][2] = ch->x1 / texWidth;
            vertices[3][3] = ch->y0 / texHeight;
            //bottom-left
            vertices[4][0] = xoffset;
            vertices[4][1] = yoffset - h + offset;
            vertices[4][2] = ch->x1 / texWidth;
            vertices[4][3] = ch->y1 / texHeight;
            //top-left
            vertices[5][0] = xoffset;
            vertices[5][1] = yoffset + offset;
            vertices[5][2] = ch->x0 / texWidth;
            vertices[5][3] = ch->y1 / texHeight;
            glBindTexture(GL_TEXTURE_2D, ch->textureId);
            glBindBuffer(GL_ARRAY_BUFFER, fontVbo);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            // 绘制四边形
            glDrawArrays(GL_TRIANGLES, 0, 6);
            
            
            xoffset += w + offsetX;
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    
    void drawOnShader(Shader& shader){
        shader.use();
        glBindVertexArray(fontVao);
        glDrawArrays(GL_TRIANGLES, 0, index);
    }

protected:
    CharacterSet characterSet[1 << 16];
    FT_Library library;
    FT_Face face;
    int yStart;
    int xStart;
    int fontSize;
    unsigned int fontVao, fontVbo;
    unsigned int index;
private:
};

#endif /* texture_find_hpp */
