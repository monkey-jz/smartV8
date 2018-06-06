//
// Created by JerryZhu on 2018/5/18.
//
#include <malloc.h>
#include "ShaderHelper.h"
#include "log.h"

GLuint compileShader(GLenum shaderType,const char* pSource)
{
    GLuint shader = glCreateShader(shaderType);
    if (shader) {
        glShaderSource(shader, 1, &pSource, NULL);
        LOGE("pSource===%s\n",
             pSource);
        glCompileShader(shader);
        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen) {
                char* buf = (char*) malloc(infoLen);
                if (buf) {
                    glGetShaderInfoLog(shader, infoLen, NULL, buf);
                    LOGE("Could not compile shader %d:\n%s\n",
                         shaderType, buf);
                    free(buf);
                }
                glDeleteShader(shader);
                shader = 0;
            }
        }
    }
    return shader;
}
static void checkGlError(const char* op) {
    for (GLint error = glGetError(); error; error
                                                    = glGetError()) {
        LOGI("after %s() glError (0x%x)\n", op, error);
    }
}

GLuint linkProgram(GLuint vertexShader,GLuint fragmentShader)
{
    GLuint program = glCreateProgram();
    if (program) {
        glAttachShader(program, vertexShader);
        checkGlError("glAttachShader");
        glAttachShader(program, fragmentShader);
        checkGlError("glAttachShader");
        glLinkProgram(program);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE) {
            GLint bufLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
            if (bufLength) {
                char* buf = (char*) malloc(bufLength);
                if (buf) {
                    glGetProgramInfoLog(program, bufLength, NULL, buf);
                    LOGE("Could not link program:\n%s\n", buf);
                    free(buf);
                }
            }
            glDeleteProgram(program);
            program = 0;
        }
    }
    return program;
}
GLuint validateProgram(GLuint program)
{
    glValidateProgram(program);
    GLint programStatus = GL_FALSE;
    glGetProgramiv(program,GL_VALIDATE_STATUS,&programStatus);
    LOGE("program validate:\n%d\n", programStatus);
    return  programStatus;
}

GLuint compileVertexShader(const char* vertexShaderSource)
{
    return compileShader(GL_VERTEX_SHADER, vertexShaderSource);
}
GLuint compileFragmentShader(const char* fragmentShaderSource)
{
    return compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
}

GLuint ShaderHelper::buildProgram(const char* vertexShaderSource,
                                  const char* fragmentShaderSource){

    GLuint program;
    // Compile the shaders.
    GLuint vertexShader = compileVertexShader(vertexShaderSource);
    GLuint fragmentShader = compileFragmentShader(fragmentShaderSource);
    // Link them into a shader program.
    program = linkProgram(vertexShader, fragmentShader);
    validateProgram(program);

    return program;
}