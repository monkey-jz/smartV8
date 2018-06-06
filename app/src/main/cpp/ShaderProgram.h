//
// Created by JerryZhu on 2018/5/18.
//

#ifndef HELLO_GL2_SHADERPROGRAM_H
#define HELLO_GL2_SHADERPROGRAM_H


#include <string>
#include <GLES2/gl2.h>

class ShaderProgram {
public:
    static const std::string VIEW;
    static const std::string MODEL;
    static const std::string PROJECTION;
    static const std::string UNIT;
    static const std::string U_TEXTURE_UNIT ;
    static const std::string A_POSITION;
    static const std::string A_COLOR;
    static const std::string A_TEX_COORD;
    static const std::string U_COLOR;
    static GLuint program;

    ShaderProgram(const char* vertexResourcePath,const char* fragmentResourcePath);

    void useProgram();
};


#endif //HELLO_GL2_SHADERPROGRAM_H
