/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// OpenGL ES 2.0 code

#include <jni.h>
#include <GLES2/gl2.h>
#include "ShaderHelper.h"
#include "ColorShaderProgram.h"
#include "TextureHelper.h"
#include "ShaderProgram.h"
#include "TextureShaderProgram.h"
#include "Table.cpp"
#include "Mallet.cpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "log.h"
#include "libplatform/libplatform.h"
#include "include/v8-platform.h"
#include <time.h>
#include "ResourceReader.h"
#include "v8Helper.h"
#include "LocalCFunction.h"
#include "Person.h"

#define OBJ_TEMPLATE(obj,isolate) Local<ObjectTemplate> obj = ObjectTemplate::New(isolate)
#define EXPOSE(obj,isolate, jsname, func) obj->Set(String::NewFromUtf8(isolate, jsname, NewStringType::kNormal).ToLocalChecked(),FunctionTemplate::New(isolate, func));

using namespace v8;

Local<Context> creatTestContext(Isolate *pIsolate);

void initV8();

Local<Object> WrapPerson(Person *person);

Local<ObjectTemplate> MakePersonTemplate(Isolate *isolate);


Local<v8::Context> context;
Global<Context> context_;
Global<Function> process_;
static Global<ObjectTemplate> person_template_;

glm::mat4 trans;
glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;
time_t timep;
image_data imageData;
char *data;
GLuint gProgram;
GLuint gvPositionHandle;
float projectionMatrix [16];
Mallet *mallet;
ColorShaderProgram * colorShaderProgram;
GLuint texture;
const char* sfsResource;
const char* svsResource;
const char* jsSource;

void callJSFunction()
{
    // Create a handle scope to keep the temporary object references.
    HandleScope handle_scope(v8Helper::GetIsolate());

    v8::Local<v8::Context> context =
            v8::Local<v8::Context>::New(v8Helper::GetIsolate(), context_);

    // Enter this processor's context so all the remaining operations
    // take place there
    Context::Scope context_scope(context);

    Person person("SexMonkey",18);
    Local<Object> person_object = WrapPerson(&person);

    // Set up an exception handler before calling the Process function
    TryCatch try_catch(v8Helper::GetIsolate());

    const int argc = 0;
    Local<Value> argv[argc] = {};
    v8::Local<v8::Function> process =
            v8::Local<v8::Function>::New(v8Helper::GetIsolate(), process_);
    Local<Value> result;
    if (!process->Call(context, context->Global(), argc, argv).ToLocal(&result)) {
        String::Utf8Value error(v8Helper::GetIsolate(), try_catch.Exception());
        LOGI("call js function error:%s",*error);
    }

}
/**
 * Utility function that wraps a C++ http person object in a
 * JavaScript object.
 */
Local<Object> WrapPerson(Person *person) {
    // Local scope for temporary handles.
    EscapableHandleScope handle_scope(v8Helper::GetIsolate());
    // Fetch the template for creating JavaScript person wrappers.
    // It only has to be created once, which we do on demand.
    if (person_template_.IsEmpty()) {
        Local<ObjectTemplate> raw_template = MakePersonTemplate(v8Helper::GetIsolate());
        person_template_.Reset(v8Helper::GetIsolate(), raw_template);
    }
    Local<ObjectTemplate> temp = Local<ObjectTemplate>::New(v8Helper::GetIsolate(),person_template_);
    // Create an empty  person wrapper.
    Local<Object> result = temp -> NewInstance(v8Helper::GetIsolate() -> GetCurrentContext()).ToLocalChecked();
    // Wrap the raw C++ pointer in an External so it can be referenced
    // from within JavaScript.
    Local<External> person_ptr = External::New(v8Helper::GetIsolate(),person);

    // Store the person pointer in the JavaScript wrapper.
    result->SetInternalField(0, person_ptr);
    // Return the result through the current handle scope.  Since each
    // of these handles will go away when the handle scope is deleted
    // we need to call Close to let one, the result, escape into the
    // outer handle scope.
    return handle_scope.Escape(result);
}

Person* UnwrapPerson(Local<Object> obj) {
    Local<External> field = Local<External>::Cast(obj->GetInternalField(0));
    void* ptr = field->Value();
    return static_cast<Person*>(ptr);
}

void GetName(Local<String> name, const PropertyCallbackInfo<Value>& info) {
    // Extract the C++ request object from the JavaScript wrapper.
    Person* person = UnwrapPerson(info.Holder());

    // Fetch the path.
    const std::string& cName = person -> getName();

    // Wrap the result in a JavaScript string and return it.
    info.GetReturnValue().Set(
            String::NewFromUtf8(info.GetIsolate(), cName.c_str(),
                                NewStringType::kNormal,
                                static_cast<int>(cName.length())).ToLocalChecked());
}

void GetAge(Local<String> name, const PropertyCallbackInfo<Value>& info) {
    // Extract the C++ request object from the JavaScript wrapper.
    Person* person = UnwrapPerson(info.Holder());

    // Fetch the path.
    const int cAge = person -> getAge();

    // Wrap the result in a JavaScript int and return it.
    info.GetReturnValue().Set(
            Integer::New(info.GetIsolate(), cAge));
}

void SetName(const FunctionCallbackInfo <Value> &args)
{
    LOGI("setName is called ...");
    Person* person = UnwrapPerson(args.Holder());
    String::Utf8Value str(args.GetIsolate(),args[0]);
    // Fetch the path.
    person ->setName(*str);
}
void SetAge(const FunctionCallbackInfo <Value> &args)
{
    LOGI("setAge is called");
    Person* person = UnwrapPerson(args.Holder());
    person -> setAge(args[0]->Uint32Value());

}


Local<ObjectTemplate> MakePersonTemplate(Isolate *isolate) {

    EscapableHandleScope handle_scope(isolate);

    Local<ObjectTemplate> result = ObjectTemplate::New(isolate);

    result->SetInternalFieldCount(1);

    // Add accessors for each of the fields of the request.
    result->SetAccessor(
            String::NewFromUtf8(isolate, "name", NewStringType::kInternalized)
                    .ToLocalChecked(),
           GetName);
    result->SetAccessor(
            String::NewFromUtf8(isolate, "age", NewStringType::kInternalized)
                    .ToLocalChecked(),
            GetAge);

    result->Set(String::NewFromUtf8(isolate, "setName", NewStringType::kNormal)
                        .ToLocalChecked(),
                FunctionTemplate::New(isolate, SetName));
    result->Set(String::NewFromUtf8(isolate, "setAge", NewStringType::kNormal)
                        .ToLocalChecked(),
                FunctionTemplate::New(isolate, SetAge));

    // Again, return the result through the current handle scope.
    return handle_scope.Escape(result);
}

void init(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    colorShaderProgram -> useProgram();
    colorShaderProgram -> setMUniforms(colorShaderProgram -> getMatrixModleLocation(),glm::value_ptr(model),texture);
    colorShaderProgram -> setMUniforms(colorShaderProgram -> getMatrixViewLocation(),glm::value_ptr(view),texture);
    colorShaderProgram -> setMUniforms(colorShaderProgram -> getMatrixProjectionLocation(),glm::value_ptr(projection),texture);
    mallet -> bindData(colorShaderProgram);
    mallet -> draw();
}

void GetTime(const FunctionCallbackInfo <Value> &args)
{
    LOGI("GetTime is called:%d",args);

}
bool  isFirst =true;

void initV8() {
    v8Helper::Initialize();
    Isolate::Scope isolate_scope(v8Helper::GetIsolate());
    // Create a stack-allocated handle scope.
    HandleScope handle_scope(v8Helper::GetIsolate());
    // Create a new context.
    Local<Context> context =creatTestContext(v8Helper::GetIsolate());
    context_.Reset(v8Helper::GetIsolate(),context);
    if (context.IsEmpty())
    {
        LOGI("Error creating context\n");
    }

    // Enter the context for compiling and running the hello world script.
    Context::Scope context_scope(context);
    // Create a string containing the JavaScript source code.
    bool result =   v8Helper::ExecuteString(context->GetIsolate(),
                                            String::NewFromUtf8(context->GetIsolate(), jsSource,

                                                                NewStringType::kNormal).ToLocalChecked(),true, true);
    LOGI("JS Script Execute Result :%d", result);
    //C++调用js类start
    Local<String> js_data = String::NewFromUtf8(v8Helper::GetIsolate(), "Point", NewStringType::kInternalized)
            .ToLocalChecked();
//    PersistentBase::
    Local<Value> js_data_value = context -> Global() -> Get(js_data);
    String::Utf8Value str(js_data_value);
    LOGI("Point = %s \n",*str);
    bool  isFunction = js_data_value -> IsFunction();
    LOGI("Point is function %d",isFunction);
    bool  isObject = js_data_value -> IsObject();
    LOGI("Point is object %d",isObject);
    Local<Object> js_data_object = Local<Object>::Cast(js_data_value);
    // var newObj = new Point(1,2);
    const int argc = 2;
    Local<Value> argv[argc] = {};
    argv[0] = Int32::New(v8Helper::GetIsolate(),7);
    argv[1] = Int32::New(v8Helper::GetIsolate(),8);
    Local<Value> newObject = js_data_object -> CallAsConstructor(context, argc, argv).ToLocalChecked();
    LOGI("Point is function %d \n",newObject -> IsFunction());
    LOGI("Point is object %d",newObject -> IsObject());
    // newObj.show();
    Local<Object> obj = Local<Object>::Cast(newObject);
    Local<String> js_func_name = String::NewFromUtf8(v8Helper::GetIsolate(), "show", NewStringType::kInternalized).ToLocalChecked();
    Local<Value>  js_func_ref = obj->Get(js_func_name);
    Local<Function> js_func = Local<Function>::Cast(js_func_ref);
    js_data_value = js_func->Call(obj, 0, NULL) ;
//    Local<String> js_pro_name = String::NewFromUtf8(v8Helper::GetIsolate(), "time", NewStringType::kInternalized).ToLocalChecked();
//    Local<Value>  js_pro_ref = obj->Get(js_pro_name);
//    Local<FunctionTemplate> timeTemp =  FunctionTemplate::New(v8Helper::GetIsolate(),GetTime)->GetFunction();

//    biketemplate->PrototypeTemplate().Set(
//            String::NewFromUtf8(isolate, "wheels"),
//            FunctionTemplate::New(isolate, MyWheelsMethodCallback)->GetFunction();
//    )


    String::Utf8Value str2(js_data_value);
    LOGI("Point = %s \n",*str2);


    //调用js方法
//    v8Helper::CallJSFunction(context, process_, v8Helper::GetIsolate(), "Process");
    Local<String> process_name =
            String::NewFromUtf8(v8Helper::GetIsolate(), "initView", NewStringType::kNormal)
                    .ToLocalChecked();
    Local<Value> process_val;
    // If there is no Process function, or if it is not a function,
    if (!context->Global()->Get(context, process_name).ToLocal(&process_val) ||
        !process_val->IsFunction()) {
        LOGI("initView is not a function\n");
    }
    // It is a function; cast it to a Function
    Local<Function> process_fun = Local<Function>::Cast(process_val);
    process_.Reset(v8Helper::GetIsolate(),process_fun);

}

namespace  v8::internal
{
    void ReadNatives(){}
    void DisposeNatives(){}
    void SetNativesFromFile(v8::StartupData *s){}
    void SetSnapshotFromFile(v8::StartupData *s){}
}
void  PersonConstructor(const FunctionCallbackInfo <Value>& args){
    LOGI("PersonConstructor is called");
    if (!args.IsConstructCall())
    {
        LOGI("args is not PersonConstructor call");
    }

    Handle<Object> object = args.This();
    HandleScope handle_scope(v8Helper::GetIsolate());
    String::Utf8Value str(args.GetIsolate(),args[0]);
    std::string name = *str;
    int age = args[1] -> Uint32Value();
    Person *person =  new Person(name,age);

    //Note that this index 0 is the internal field we created in the template!
    object -> SetInternalField(0,v8::External::New(v8Helper::GetIsolate(),person));
}
// Create a template for the global object.
//    OBJ_TEMPLATE(global,isolate);
//    EXPOSE(global,isolate,"print",LocalCFunction::print);
//    EXPOSE(global,isolate,"add",LocalCFunction::add);
//    EXPOSE(global,isolate,"draw",init);

Local<Context> creatTestContext(Isolate *isolate) {
    // Create a template for the global object.
    OBJ_TEMPLATE(global,isolate);
    EXPOSE(global,isolate,"print",LocalCFunction::print);
    EXPOSE(global,isolate,"add",LocalCFunction::add);
    EXPOSE(global,isolate,"draw",init);
    //Create the function template for the constructor, and point it to our constructor,
    Handle<FunctionTemplate> person_template = FunctionTemplate::New(v8Helper::GetIsolate(),PersonConstructor);
    //We can tell the scripts what type to report. In this case, just Person will do.
    person_template->SetClassName(String::NewFromUtf8(v8Helper::GetIsolate(), "Person", NewStringType::kNormal)
                                           .ToLocalChecked());
    //This template is the unique properties of this type, we can set
    //functions, getters, setters, and values directly on each new Person() object.
    Handle<ObjectTemplate> person = person_template -> InstanceTemplate();
    //Again, this is to store the c++ object for use in callbacks.
    person -> SetInternalFieldCount(1);
    person -> SetAccessor(
            String::NewFromUtf8(isolate, "name", NewStringType::kInternalized)
                    .ToLocalChecked(),
            GetName);
    person -> SetAccessor(
            String::NewFromUtf8(isolate, "age", NewStringType::kInternalized)
                    .ToLocalChecked(),
            GetAge);

    person -> Set(String::NewFromUtf8(isolate, "setName", NewStringType::kNormal)
                        .ToLocalChecked(),
                FunctionTemplate::New(isolate, SetName));
    person -> Set(String::NewFromUtf8(isolate, "setAge", NewStringType::kNormal)
                        .ToLocalChecked(),
                FunctionTemplate::New(isolate, SetAge));

    //Finally, we can tell the global scope that it now has a 'function' called Person,
    //and that this function returns the template object above.
    global -> Set(String::NewFromUtf8(isolate, "Person", NewStringType::kNormal)
                          .ToLocalChecked(),person_template);

    return Context::New(isolate,0,global);
}



//向MakeWeak注册的callback.
void PersonWeakReferenceCallback(Persistent<Value> object
        , void * param) {
    if (Person* person = static_cast<Person*>(param)) {
        delete person;
    }
}
//将C++指针通过External保存为Persistent对象，避免指针被析构
Local<Value> MakeWeakPerson(void* parameter) {
//    Persistent<External> persistentCloudApp =
//            Persistent<External>::New(External::New(v8Helper::GetIsolate(),parameter));
//MakeWeak非常重要，当JS世界new一个CloudApp对象之后
//C++也必须new一个对应的指针。
//JS对象析构之后必须想办法去析构C++的指针，可以通过MakeWeak来实现，
//MakeWeak的主要目的是为了检测Persistent Handle除了当前Persistent
//的唯一引用外，没有其他的引用，就可以析构这个Persistent Handle了，
//同时调用MakeWeak的callback。这是我们可以再这个callback中delete
//C++指针
//    persistentCloudApp.MakeWeak(parameter, PersonWeakReferenceCallback);
//    Local<External> person_ptr = External::New(v8Helper::GetIsolate(),parameter);
    return Local<Value>();
}


void SetUpPersonInterface(Local<ObjectTemplate> local) {
    //Create the function template for the constructor, and point it to our constructor,

//    Local<External> person_ptr = External::New(v8Helper::GetIsolate(),person);
}

static void _onSurfaceCreated(JNIEnv *env, jclass clazz)
{
    LOGI("native method onSurfaceCreated ...");

    initV8();

    glClearColor(1.0f, 1.0f, 0.0f, 0.0f);
    mallet =  Mallet::getInstance();

    glEnable(GL_DEPTH_TEST);

    if(isFirst)
    {

        model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        // 注意，我们将矩阵向我们要进行移动场景的反方向移动。
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

        projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);

        isFirst = false;
    }

    colorShaderProgram =  ColorShaderProgram::getInstance(svsResource,sfsResource);
    texture = TextureHelper::loadTexture(imageData);
    LOGI("texture =  %d",texture);

}

static void _onSurfaceChanged(JNIEnv *env, jclass clazz,jint w,jint h)
{
    LOGI("native method onSurfaceChanged ...");
    glViewport(0,0,w,h);
}

static void _onDrawFrame(JNIEnv *env, jclass clazz)
{
//  LOGI("native method onDrawFrame ...");
    callJSFunction();

}
static void _loadShaderSource(JNIEnv *env, jclass clazz,jobject assets)
{
    LOGI("native method loadShaderSource ...");
    svsResource = ResourceReader::loadShaderSource(env,assets,"simple_vertex_shader.vs");
    sfsResource = ResourceReader::loadShaderSource(env,assets,"simple_fragment_shader.fs");
    jsSource = ResourceReader::loadShaderSource(env,assets,"script/main.js");

}
static void _initImage(JNIEnv *env, jclass clazz,jbyteArray bitmap_,jint width,jint height)
{
    LOGI("native method initImage ...");
    LOGI("width : %d hieght : %d",width,height);

    data = (char *) env->GetByteArrayElements(bitmap_, 0);
    imageData.piexl = data;
    imageData.height = height;
    imageData.width = width;

    env->ReleaseByteArrayElements(bitmap_, (jbyte *) data, 0);

}

static void _onDestory(JNIEnv *env, jclass clazz)
{
 LOGI("native method onDestory ...");
    v8Helper::ShutDown();
}

static JNINativeMethod _methodTable[] = {
        { "onSurfaceChanged", "(II)V", (void*)_onSurfaceChanged },
        { "onDrawFrame", "()V", (void*)_onDrawFrame },
        { "loadShaderSource", "(Landroid/content/res/AssetManager;)V", (void*)_loadShaderSource},
        { "onDestory", "()V", (void*)_onDestory },
        { "initImage", "([BII)V", (void*)_initImage },
        { "onSurfaceCreated", "()V", (void*)_onSurfaceCreated}
};

int JNI_OnLoad(JavaVM* vm, void* reserved)
{
    LOGI("JNI_OnLoad ...");
    JNIEnv* env = 0;
    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK)
    {
        return JNI_ERR;
    }
    jclass cls = env -> FindClass("com/android/gl2v8/GL2JNILib");

    if (cls == 0)
    {
        LOGI("JNI_OnLoad cls null");
        return JNI_ERR;
    }

    int len = sizeof(_methodTable) / sizeof(_methodTable[0]);
    if (JNI_OK != env->RegisterNatives(cls, _methodTable, len))
    {
        LOGE( "_registerNativeMethods: failed");
        if (env->ExceptionCheck())
        {
            env->ExceptionClear();
        }
    }

    return JNI_VERSION_1_4;
}
