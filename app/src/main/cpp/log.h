//
// Created by JerryZhu on 2018/5/18.
//
#include <android/log.h>

#ifndef HELLO_GL2_LOG_H
#define HELLO_GL2_LOG_H

#define  LOG_TAG    "libgl2jni"

#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
typedef struct _image_data_
{
    char *piexl;
    int width;
    int height;

} image_data;

#endif //HELLO_GL2_LOG_H

