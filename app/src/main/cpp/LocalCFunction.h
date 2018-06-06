//
// Created by JerryZhu on 2018/5/28.
//
#include "v8.h"
#include "log.h"
#ifndef HELLO_GL2_LOCALCFUNCTION_H
#define HELLO_GL2_LOCALCFUNCTION_H
using namespace v8;

class LocalCFunction {
public:
    static void add(const FunctionCallbackInfo<Value>& args);
    static void print(const FunctionCallbackInfo<Value>& args);
    static void draw(const FunctionCallbackInfo<Value>& args);
    static void getName(Local<String> name, const PropertyCallbackInfo<Value>& info);
};


#endif //HELLO_GL2_LOCALCFUNCTION_H
