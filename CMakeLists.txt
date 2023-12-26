cmake_minimum_required(VERSION 3.16)
project(qcpp LANGUAGES CXX)
set(CMAKE_PREFIX_PATH "C:\\Qt\\Qt5.14.2\\5.14.2\\msvc2017")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

if (MSVC)
    add_compile_options(/source-charset:utf-8)
    add_compile_options(/EHsc)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rd/)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core)

set(PROJECT_SOURCES
        main.cpp
)

add_executable(qcpp ${PROJECT_SOURCES})
set_target_properties(qcpp
    PROPERTIES
        WIN32_EXECUTABLE TRUE
)
target_link_libraries(qcpp PUBLIC
        Qt${QT_VERSION_MAJOR}::Core
)