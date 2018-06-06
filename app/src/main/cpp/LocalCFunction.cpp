//
// Created by JerryZhu on 2018/5/28.
//

#include "LocalCFunction.h"
void LocalCFunction::add(const FunctionCallbackInfo <Value> &args)
{
    int a = args[0]->Uint32Value();
    int b = args[1]->Uint32Value();
    args.GetReturnValue().Set(
            Integer::New(args.GetIsolate(), a+b));
    LOGI("add is called");

}
void LocalCFunction::print(const FunctionCallbackInfo <Value> &args)
{
    bool first = true;
    for (int i = 0; i < args.Length(); i++) {
        HandleScope handle_scope(args.GetIsolate());
        if (first) {
            first = false;
        } else {
            printf(" 1111");
        }
        String::Utf8Value str(args.GetIsolate(),args[i]);
//        const char* cstr = ToCString(str);
        LOGI("print is called : %s",*str);
    }

    fflush(stdout);
}
void LocalCFunction::draw(const FunctionCallbackInfo<Value> &args)
{

}
void LocalCFunction::getName(Local<String> name, const PropertyCallbackInfo<Value> &info)
{

}