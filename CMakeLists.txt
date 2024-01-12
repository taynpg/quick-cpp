cmake_minimum_required(VERSION 3.16)
project(qcpp LANGUAGES CXX)

set(CMAKE_PREFIX_PATH 
    "C:\\binary\\qt\\QT6.5.3_x64"
    "C:\\binary\\qt\\QT6.5.3_x86"
    "C:\\binary\\cpp\\Boost\\1_84"
    "/opt/homebrew/Cellar/qt@5/5.15.12"
    "/Users/tangyunpeng/mdev/Boost"
)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

if (MSVC)
    #add_compile_options(/source-charset:utf-8)
    add_compile_options(/EHsc)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rd/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Widgets)

# Unix系统boost编译的是静态库
if (UNIX)
    set(Boost_USE_STATIC_LIBS ON)
else()
    set(Boost_USE_STATIC_LIBS OFF)
endif()
find_package(Boost REQUIRED filesystem)
include_directories(${Boost_INCLUDE_DIR})

set(PROJECT_SOURCES
    pub_handle.h pub_handle.cpp
    template.qrc
    main.cpp
    src/config.cpp
    src/project_opr.cpp
    MainWidget.h
    MainWidget.cpp
    MainWidget.ui
    resource/ico.rc
    shortkey.h shortkey.cpp shortkey.ui
)

add_executable(qcpp ${PROJECT_SOURCES})
set_target_properties(qcpp
    PROPERTIES
        WIN32_EXECUTABLE TRUE
)
target_link_libraries(qcpp PUBLIC
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Widgets
        ${Boost_LIBRARIES}
)
