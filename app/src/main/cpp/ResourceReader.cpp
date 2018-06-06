//
// Created by JerryZhu on 2018/5/25.
//
#include <sys/types.h>
#include <stdlib.h>
#include <android/asset_manager_jni.h>
#include <android/asset_manager.h>
#include "ResourceReader.h"
#include "log.h"

const char* ResourceReader::loadShaderSource(JNIEnv *env, jobject assetManager, const char *name)
{
    AAssetManager* mgr = AAssetManager_fromJava(env, assetManager);
    if(mgr==NULL)
    {
        LOGI(" %s","AAssetManager==NULL");
        return 0;
    }
    AAsset* asset = AAssetManager_open(mgr, name,AASSET_MODE_UNKNOWN);
    if(asset==NULL)
    {
        return 0;
    }
    off_t bufferSize = AAsset_getLength(asset);
    char *buffer=(char *)malloc(bufferSize+1);
    buffer[bufferSize]=0;
    int numBytesRead = AAsset_read(asset, buffer, bufferSize);
//    free(buffer);
    AAsset_close(asset);
    return buffer;
}