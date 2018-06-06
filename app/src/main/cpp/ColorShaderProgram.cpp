//
// Created by JerryZhu on 2018/5/18.
//

#include "ColorShaderProgram.h"
#include "log.h"

GLuint ColorShaderProgram::aPositionlocation;
GLuint ColorShaderProgram::uMatrixMODELLocation;
GLuint ColorShaderProgram::uMatrixVIEWLocation;
GLuint ColorShaderProgram::uMatrixPROJECTIONLocation;
GLuint ColorShaderProgram::aColorLocation;
GLuint ColorShaderProgram::aTexCoordLocation;
GLuint ColorShaderProgram::uTexCoordUnitLocation;
GLuint ColorShaderProgram::uUnitMatrixLocation;

ColorShaderProgram::ColorShaderProgram(const char *vertexResourcePath,
                                       const char *fragmentResourcePath) : ShaderProgram(
        vertexResourcePath, fragmentResourcePath)
{
    aPositionlocation = glGetAttribLocation(program, A_POSITION.c_str());
    uMatrixMODELLocation = glGetUniformLocation(program,MODEL.c_str());
    uMatrixVIEWLocation = glGetUniformLocation(program,VIEW.c_str());
    uMatrixPROJECTIONLocation = glGetUniformLocation(program,PROJECTION.c_str());
    aColorLocation = glGetAttribLocation(program, A_COLOR.c_str());
    aTexCoordLocation = glGetAttribLocation(program, A_TEX_COORD.c_str());
    uTexCoordUnitLocation = glGetAttribLocation(program, U_TEXTURE_UNIT.c_str());
    uUnitMatrixLocation = glGetUniformLocation(program, UNIT.c_str());

}

void ColorShaderProgram::setMUniforms(GLint location, const float *matrix, GLuint textureId)
{
//     Pass the matrix into the shader program.
    glUniformMatrix4fv(location, 1, false, matrix);
    // Set the active texture unit to texture unit 0.
    glActiveTexture(GL_TEXTURE0);
//    // Bind the texture to this unit.
    glBindTexture(GL_TEXTURE_2D, textureId);
// Tell the texture uniform sampler to use this texture in the shader by
// telling it to read from texture unit 0.
    glUniform1i(location, 0);
}
void ColorShaderProgram::setMUniforms(GLint location, const float *matrix)
{
//     Pass the matrix into the shader program.
    glUniformMatrix4fv(uUnitMatrixLocation, 1, false, matrix);

}

GLuint ColorShaderProgram::getMatrixModleLocation()
{
    return uMatrixMODELLocation;
}
GLuint ColorShaderProgram::getMatrixViewLocation()
{
    return uMatrixVIEWLocation;
}
GLuint ColorShaderProgram::getMatrixProjectionLocation()
{
    return uMatrixPROJECTIONLocation;
}
GLuint ColorShaderProgram::getUnitMatrixLocation(){
    return uUnitMatrixLocation;
}
GLuint ColorShaderProgram::getColorAttributeLocation()
{
    return aColorLocation;
}

GLuint ColorShaderProgram::getPositionAttributeLocation(){
    return aPositionlocation;
}
GLuint ColorShaderProgram::getTexCoordAttributeLocation(){
    return aTexCoordLocation;
}
ColorShaderProgram* ColorShaderProgram::getInstance(const char *vertexResourcePath,
                                                    const char *fragmentResourcePath)
{
    ColorShaderProgram *colorShaderProgram  = new ColorShaderProgram(vertexResourcePath,fragmentResourcePath);
    return colorShaderProgram;
}
void ColorShaderProgram::useProgram() {
    ShaderProgram::useProgram();
}

