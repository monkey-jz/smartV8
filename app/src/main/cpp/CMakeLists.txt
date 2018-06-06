cmake_minimum_required(VERSION 3.4.1)

# now build app's shared lib
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

add_library(v8_base SHARED  IMPORTED)
set_target_properties( v8_base
                       PROPERTIES IMPORTED_LOCATION
                       ${CMAKE_SOURCE_DIR}/../jniLibs/${ANDROID_ABI}/libv8_base.a)

add_library(v8_snapshot SHARED  IMPORTED)
set_target_properties( v8_snapshot
                       PROPERTIES IMPORTED_LOCATION
                       ${CMAKE_SOURCE_DIR}/../jniLibs/${ANDROID_ABI}/libv8_snapshot.a)

add_library(gl2v8 SHARED
            ColorShaderProgram.cpp
            Mallet.cpp
            ShaderHelper.cpp
            ShaderProgram.cpp
            Table.cpp
            TextureHelper.cpp
            TextureShaderProgram.cpp
            ResourceReader.cpp
            v8Helper.cpp
            LocalCFunction.cpp
            Person.cpp
            gl_code.cpp)
                        
find_library( # Sets the name of the path variable.
              lib-log

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log
              android
              )
include_directories(${CMAKE_SOURCE_DIR}/glm)
include_directories(${CMAKE_SOURCE_DIR}/include)

# add lib dependencies
target_link_libraries(gl2v8
                      android
                      EGL
                      GLESv2
                      v8_base
                      v8_snapshot
                      ${lib-log}
                      )

