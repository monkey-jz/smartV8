//
// Created by JerryZhu on 2018/5/25.
//

#include <assert.h>
#include <libplatform/libplatform.h>
#include "v8Helper.h"
#include "log.h"

 bool v8Helper::ExecuteString(Isolate *isolate, Local<String> source, bool print_result,
                              bool report_exceptions)
 {

     HandleScope handle_scope(isolate);
     TryCatch try_catch(isolate);

     Local<Context> context(isolate->GetCurrentContext());
     Local<Script> script;
     if (!Script::Compile(context, source).ToLocal(&script)) {
         // Print errors that happened during compilation.
         if (report_exceptions)
             v8Helper::ReportException(isolate, &try_catch);
         return false;
     } else {
         Local<Value> result;
         if (!script->Run(context).ToLocal(&result)) {
             assert(try_catch.HasCaught());
             // Print errors that happened during execution.
             if (report_exceptions)
                 v8Helper::ReportException(isolate, &try_catch);
             return false;
         } else {
             assert(!try_catch.HasCaught());
             if (print_result && !result->IsUndefined()) {
                 // If all went well and the result wasn't undefined then print
                 // the returned value.
                 String::Utf8Value str(isolate, result);
                 LOGI("=========== %s\n", *str);
             }
             return true;
         }
     }
 }
void v8Helper::ReportException(Isolate *isolate, TryCatch *try_catch)
{
    HandleScope handle_scope(isolate);
    String::Utf8Value exception(isolate, try_catch->Exception());
//    const char *exception_string = ToCString(exception);
    Local<Message> message = try_catch->Message();
    if (message.IsEmpty()) {
        // V8 didn't provide any extra information about this error; just
        // print the exception.
        LOGI("exception_string ; %s\n", *exception);
    } else {
        // Print (filename):(line number): (message).
        String::Utf8Value filename(isolate,
                                       message->GetScriptOrigin().ResourceName());
        Local<Context> context(isolate->GetCurrentContext());
        int linenum = message->GetLineNumber(context).FromJust();
        LOGI("exception_string : %s:%i: %s\n",  *filename, linenum, *exception);
        // Print line of source code.
        String::Utf8Value sourceline(
                isolate, message->GetSourceLine(context).ToLocalChecked());
        LOGI("stderr :%s\n",*sourceline);
        // Print wavy underline (GetUnderline is deprecated).
        int start = message->GetStartColumn(context).FromJust();
        for (int i = 0; i < start; i++) {
            fprintf(stderr, " ");
        }
        int end = message->GetEndColumn(context).FromJust();
        for (int i = start; i < end; i++) {
            fprintf(stderr, "^");
        }
        fprintf(stderr, "\n");
        Local<Value> stack_trace_string;
        if (try_catch->StackTrace(context).ToLocal(&stack_trace_string) &&
            stack_trace_string->IsString() &&
            Local<v8::String>::Cast(stack_trace_string)->Length() > 0) {
            String::Utf8Value stack_trace(isolate, stack_trace_string);
            LOGI("exception_string : %s\n\n",*stack_trace);
        }
    }

}

void v8Helper::CallJSFunction(Local<Context> context, Global<Function> process_, Isolate *isolate, const char* name)
{
    Local<String> process_name =
            String::NewFromUtf8(isolate, name, NewStringType::kNormal)
                    .ToLocalChecked();
    Local<Value> process_val;
    // If there is no Process function, or if it is not a function,
    // bail out
    if (!context->Global()->Get(context, process_name).ToLocal(&process_val) ||
        !process_val->IsFunction()) {
        LOGI("%s is not a function\n",name);
    }
    // It is a function; cast it to a Function
    Local<Function> process_fun = Local<Function>::Cast(process_val);
    //Store the function in a Global handle, since we also want
    process_.Reset(GetIsolate(),process_fun);

}

Isolate::CreateParams create_params;
Isolate *isolate;

void v8Helper::Initialize()
{

    Platform *platform = platform::CreateDefaultPlatform();
    V8::InitializePlatform(platform);
    V8::Initialize();

    create_params.array_buffer_allocator =
            ArrayBuffer::Allocator::NewDefaultAllocator();
    isolate =Isolate::New(create_params);
//    Isolate::Scope isolate_scope(isolate);
//    // Create a stack-allocated handle scope.
//    HandleScope handle_scope(isolate);

}
Isolate* v8Helper::GetIsolate()
{
    return isolate;
}

void v8Helper::ShutDown()
{
    GetIsolate()->Dispose();
    V8::Dispose();
    V8::ShutdownPlatform();
    delete create_params.array_buffer_allocator;
}
