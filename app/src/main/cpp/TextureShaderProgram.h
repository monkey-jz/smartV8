//
// Created by JerryZhu on 2018/5/18.
//
#include "ShaderProgram.h"
#ifndef HELLO_GL2_TEXTURESHADERPROGRAM_H
#define HELLO_GL2_TEXTURESHADERPROGRAM_H


class TextureShaderProgram :ShaderProgram{

private:

    // Uniform locations
    static GLuint uMatrixLocation;
    static GLuint uTextureUnitLocation;
    // Attribute locations
    static GLuint aPositionLocation;
    static GLuint aTextureCoordinatesLocation;
public:
    TextureShaderProgram(const char *vertexResourcePath, const char *fragmentResourcePath);
    void setUniforms(const float* ,GLuint);
    void setUniforms(const float* );
    GLuint getPositionAttributeLocation();
    GLuint getTextureCoordinatesAttributeLocation();
    static TextureShaderProgram* getInstance(const char *vertexResourcePath, const char *fragmentResourcePath);
    void useProgram();
};


#endif //HELLO_GL2_TEXTURESHADERPROGRAM_H
