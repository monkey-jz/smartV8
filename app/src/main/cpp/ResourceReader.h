//
// Created by JerryZhu on 2018/5/25.
//

#ifndef HELLO_GL2_RESOURCEREADER_H
#define HELLO_GL2_RESOURCEREADER_H


#include <jni.h>

class ResourceReader {
    public:
    static const char *loadShaderSource(JNIEnv *env, jobject assetManager, const char* name);
};


#endif //HELLO_GL2_RESOURCEREADER_H
