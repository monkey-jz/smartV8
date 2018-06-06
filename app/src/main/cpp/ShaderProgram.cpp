//
// Created by JerryZhu on 2018/5/18.
//

#include "ShaderProgram.h"
#include "ShaderHelper.h"

const std::string ShaderProgram::U_TEXTURE_UNIT = "ourTexture";
const std::string ShaderProgram::A_POSITION = "a_Position";
const std::string ShaderProgram::A_COLOR = "a_Color";
const std::string ShaderProgram::A_TEX_COORD = "a_TexCoord";
const std::string ShaderProgram::U_COLOR = "u_Color";
const std::string ShaderProgram::VIEW = "view";
const std::string ShaderProgram::PROJECTION = "projection";
const std::string ShaderProgram::MODEL = "model";
const std::string ShaderProgram::UNIT = "unitMatrix";
GLuint ShaderProgram::program;

ShaderProgram::ShaderProgram(const char *vertexResource, const char *fragmentResource)
{
    program = ShaderHelper::buildProgram(
              vertexResource,
              fragmentResource);
}
void ShaderProgram::useProgram()
{
    glUseProgram(program);
}

