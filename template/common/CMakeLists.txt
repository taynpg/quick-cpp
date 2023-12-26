cmake_minimum_required (VERSION 3.8)
project (replace)
set(CMAKE_CXX_STANDARD 11)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()
if (MSVC)
    add_compile_options(/source-charset:utf-8)
    add_compile_options(/EHsc)
    add_compile_options(-D_CRT_SECURE_NO_WARNINGS)
endif()

add_executable(replace main.cpp)