//
// Created by JerryZhu on 2018/5/18.
//

#include "TextureHelper.h"
#include "log.h"


GLuint TextureHelper::loadTexture(image_data image)
{
    GLuint texture;
    glGenTextures(1,&texture);
    //绑定纹理对象
    glBindTexture(GL_TEXTURE_2D,texture);
//    //纹理环绕
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    //纹理过滤
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width, image.height, 0, GL_RGB, GL_UNSIGNED_BYTE, image.piexl);
    glGenerateMipmap(GL_TEXTURE_2D);

    return  texture;
}