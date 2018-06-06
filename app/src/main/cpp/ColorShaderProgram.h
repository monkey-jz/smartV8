//
// Created by JerryZhu on 2018/5/18.
//
#include "ShaderProgram.h"
#ifndef HELLO_GL2_COLORSHADERPROGRAM_H
#define HELLO_GL2_COLORSHADERPROGRAM_H


class ColorShaderProgram :ShaderProgram{

private: static GLuint aPositionlocation;
          static GLuint uMatrixMODELLocation;
          static GLuint uMatrixPROJECTIONLocation;
          static GLuint uMatrixVIEWLocation;
          static GLuint aColorLocation;
          static GLuint aTexCoordLocation;
          static GLuint uTexCoordUnitLocation;
          static GLuint uUnitMatrixLocation;
public:

    ColorShaderProgram(const char *vertexResourcePath, const char *fragmentResourcePath);


    void setMUniforms(GLint  location,const float * matrix, GLuint textureId);
    GLuint getPositionAttributeLocation();
    GLuint getColorAttributeLocation();
    GLuint getTexCoordAttributeLocation();
    static ColorShaderProgram* getInstance(const char *vertexResourcePath, const char *fragmentResourcePath);
    void useProgram();

    GLuint getMatrixModleLocation();

    GLuint getMatrixViewLocation();

    GLuint getMatrixProjectionLocation();
    GLuint getUnitMatrixLocation();

    void setMUniforms(GLint location, const float *matrix);
};



#endif //HELLO_GL2_COLORSHADERPROGRAM_H
