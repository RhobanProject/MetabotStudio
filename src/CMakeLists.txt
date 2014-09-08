cmake_minimum_required(VERSION 2.8)
project(metabot)
find_package (Threads)

set (CMAKE_CXX_FLAGS "-std=c++0x -Wall -pedantic -U__STRICT_ANSI__")

set(ALL_SOURCES
    util.cpp
    Backend.cpp
    Component.cpp
    ComponentInstance.cpp
    ComponentParameter.cpp
    CSG.cpp
    TransformMatrix.cpp
    Vector.cpp
    )

include_directories("${CMAKE_SOURCE_DIR}/src")

add_library(libmetabot ${ALL_SOURCES})

target_link_libraries(libmetabot ${CMAKE_THREAD_LIBS_INIT})

add_executable(metabot "main.cpp")
target_link_libraries(metabot libmetabot)

install(TARGETS metabot DESTINATION bin)
