//
// Created by JerryZhu on 2018/5/25.
//

#ifndef HELLO_GL2_V8HELPER_H
#define HELLO_GL2_V8HELPER_H


#include "include/v8.h"
using namespace v8;

class v8Helper {

public:

    static void ReportException(Isolate* isolate, TryCatch* try_catch);
    static bool ExecuteString(Isolate* isolate, Local<String> source,
                               bool print_result,
                               bool report_exceptions);
    static void CallJSFunction(Local<Context> context, Global<Function> function, Isolate *isolate,
                                  const char * name);
    static void Initialize();
    static Isolate* GetIsolate();
    static void ShutDown();

};


#endif //HELLO_GL2_V8HELPER_H
