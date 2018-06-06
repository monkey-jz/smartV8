//
// Created by JerryZhu on 2018/5/18.
//

#include <GLES2/gl2.h>
#include "TextureShaderProgram.h"


class Table
{
private:
     int POSITION_COMPONENT_COUNT = 2;
     int TEXTURE_COORDINATES_COMPONENT_COUNT = 2;
     int STRIDE = (POSITION_COMPONENT_COUNT
                                   + TEXTURE_COORDINATES_COMPONENT_COUNT) * 4;
     GLfloat VERTEX_DATA  [24] = {
            // Order of coordinates: X, Y, S, T
            // Triangle Fan
            0.0f,    0.0f,     0.5f, 0.5f,
            -0.5f, -0.8f,  0.0f, 0.9f,
            0.5f,  -0.8f,  1.0f, 0.9f,
            0.5f,  0.8f,   1.0f, 0.1f,
            -0.5f, 0.8f,   0.0f, 0.1f,
            -0.5f, -0.8f,  0.0f, 0.9f
    };

public:
    Table() {}

    static Table* getInstance(){
        static Table table;
        return &table;
    }

    void bindData(TextureShaderProgram *textureShaderProgram)
    {
        glVertexAttribPointer(textureShaderProgram -> getPositionAttributeLocation(), POSITION_COMPONENT_COUNT, GL_FLOAT,
                              false, STRIDE, VERTEX_DATA);
        glEnableVertexAttribArray(textureShaderProgram -> getPositionAttributeLocation());

        glVertexAttribPointer(textureShaderProgram -> getTextureCoordinatesAttributeLocation(), TEXTURE_COORDINATES_COMPONENT_COUNT, GL_FLOAT,
                              false, STRIDE, VERTEX_DATA);
        glEnableVertexAttribArray(textureShaderProgram -> getTextureCoordinatesAttributeLocation());
    }

    void draw()
    {
        glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
    }
};