//
// Created by JerryZhu on 2018/5/18.
//

#include "TextureShaderProgram.h"

GLuint TextureShaderProgram::uMatrixLocation;
GLuint TextureShaderProgram::uTextureUnitLocation;
GLuint TextureShaderProgram::aPositionLocation;
GLuint TextureShaderProgram::aTextureCoordinatesLocation;


TextureShaderProgram::TextureShaderProgram(const char *vertexResource,
                                           const char *fragmentResource) : ShaderProgram(
        vertexResource, fragmentResource)
{
    uTextureUnitLocation = glGetUniformLocation(program,U_TEXTURE_UNIT.c_str());
    aPositionLocation = glGetAttribLocation(program,A_POSITION.c_str());
    aTextureCoordinatesLocation = glGetAttribLocation(program,A_TEX_COORD.c_str());
}
void TextureShaderProgram::setUniforms(const float * matrix, GLuint textureId)
{
  // Pass the matrix into the shader program.
    glUniformMatrix4fv(uMatrixLocation, 1, false, matrix);
    // Set the active texture unit to texture unit 0.
    glActiveTexture(GL_TEXTURE0);
   // Bind the texture to this unit.
    glBindTexture(GL_TEXTURE_2D, textureId);
// Tell the texture uniform sampler to use this texture in the shader by
// telling it to read from texture unit 0.
    glUniform1i(uTextureUnitLocation, 0);
}

void TextureShaderProgram::setUniforms(const float * matrix)
{
    glUniformMatrix4fv(uMatrixLocation, 1, false, matrix);
}

GLuint TextureShaderProgram::getPositionAttributeLocation()
{
    return aPositionLocation;
}

GLuint TextureShaderProgram::getTextureCoordinatesAttributeLocation(){
    return aTextureCoordinatesLocation;
}
TextureShaderProgram* TextureShaderProgram::getInstance(const char *vertexResource,
                                                        const char *fragmentResource)
{
    static TextureShaderProgram textureShaderProgram(vertexResource,
                                                     fragmentResource);
    return &textureShaderProgram;
}
void TextureShaderProgram::useProgram() {
    ShaderProgram::useProgram();
}
