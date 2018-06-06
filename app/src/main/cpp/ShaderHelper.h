//
// Created by JerryZhu on 2018/5/18.
//
#include <string>
#include <GLES2/gl2.h>

#ifndef HELLO_GL2_SHADERHELPER_H
#define HELLO_GL2_SHADERHELPER_H

class ShaderHelper
{
     public:
         static GLuint buildProgram(const char* vertexShaderSource,const char* fragmentShaderSource);

};


#endif //HELLO_GL2_SHADERHELPER_H
