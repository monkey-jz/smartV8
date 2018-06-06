#include <GLES2/gl2.h>
#include "ColorShaderProgram.h"
#include "log.h"
//
// Created by JerryZhu on 2018/5/18.
//
class Mallet
{
  public:
    int POSITION_COMPONENT_COUNT = 3;
    int COLOR_COMPONENT_COUNT = 3;
    int TEXTURE_COMPONENT_COUNT = 2;
    int STRIDE  = (POSITION_COMPONENT_COUNT + COLOR_COMPONENT_COUNT + TEXTURE_COMPONENT_COUNT) * 4;

    GLfloat VERTEX_DATA [36]= {
            // Order of coordinates: X, Y,
            // 位置
            0.5f, 0.5f, 0.0f,  1.0f,0.0f,0.0f, 1.0f, 1.0f,  // 右上
            0.5f, -0.5f, 0.0f, 0.0f,1.0f,0.0f, 1.0f, 0.0f, // 右下
            -0.5f,  -0.5f, 0.0f, 0.0f,0.0f,1.0f, 0.0f, 0.0f,   // 左下
            -0.5f,  0.5f, 0.0f,  1.0f,0.0f,0.0f, 0.0f, 1.0f,  // 左上
    };
    unsigned  int indices[6] = {

            0,1,3,//第一个三角形

            1,2,3//第二个三角形
    };

   public:
    Mallet() {}

    static Mallet* getInstance(){
       static Mallet mallet;
        return &mallet;
    }
    unsigned  int EBO;
    void bindData(ColorShaderProgram *colorShaderProgram)
    {
        unsigned int VBO;
        glGenBuffers(1, &VBO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(VERTEX_DATA), VERTEX_DATA, GL_STATIC_DRAW);


        glGenBuffers(1,&EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),indices,GL_STATIC_DRAW);

        glVertexAttribPointer(colorShaderProgram -> getPositionAttributeLocation(),POSITION_COMPONENT_COUNT, GL_FLOAT,
                              false, STRIDE,(void*)0);
        glEnableVertexAttribArray(colorShaderProgram -> getPositionAttributeLocation());

        glVertexAttribPointer(colorShaderProgram -> getColorAttributeLocation(),COLOR_COMPONENT_COUNT, GL_FLOAT,
                              false, STRIDE,(void*)(POSITION_COMPONENT_COUNT* sizeof(float)));
        glEnableVertexAttribArray(colorShaderProgram -> getColorAttributeLocation());

        glVertexAttribPointer(colorShaderProgram -> getTexCoordAttributeLocation(),TEXTURE_COMPONENT_COUNT, GL_FLOAT,
                              false, STRIDE,(void*)((POSITION_COMPONENT_COUNT+COLOR_COMPONENT_COUNT)* sizeof(float)));
        glEnableVertexAttribArray(colorShaderProgram -> getTexCoordAttributeLocation());

    }
    void draw()
    {
        glBindBuffer(GL_ARRAY_BUFFER,0);
        glDrawElements(GL_TRIANGLES, 6,GL_UNSIGNED_INT,0);
    }

};

