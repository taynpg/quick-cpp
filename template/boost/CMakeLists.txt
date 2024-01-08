cmake_minimum_required (VERSION 3.8)
project (replaceB)
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_PREFIX_PATH 
    "replaceA"
)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()
if (MSVC)
    add_compile_options(/source-charset:utf-8)
    add_compile_options(/EHsc)
    add_compile_options(-D_CRT_SECURE_NO_WARNINGS)
endif()

set(Boost_USE_STATIC_LIBS replaceC)
find_package(Boost REQUIRED filesystem)
include_directories(${Boost_INCLUDE_DIR})

add_executable(replaceB main.cpp)
target_link_libraries(replaceB PRIVATE
    ${Boost_LIBRARIES}
)