cmake_minimum_required(VERSION 2.8)
project(metabot)
find_package (Threads)

set (CMAKE_CXX_FLAGS "-std=c++0x -Wall -pedantic -U__STRICT_ANSI__")

set(ALL_SOURCES
    sha1.cpp
    util.cpp
    
    AnchorPoint.cpp
    Backend.cpp
    Cache.cpp
    Component.cpp
    ComponentInstance.cpp
    ComponentParameter.cpp
    CSG.cpp
    CSGNode.cpp
    Model.cpp
    Part.cpp
    TransformMatrix.cpp
    Vector.cpp
    )

include_directories("${CMAKE_SOURCE_DIR}")
include_directories("${CMAKE_SOURCE_DIR}/json/include")
add_subdirectory("${CMAKE_SOURCE_DIR}/json")

add_library(libmetabot ${ALL_SOURCES})

target_link_libraries(libmetabot ${CMAKE_THREAD_LIBS_INIT} json)

add_executable(metabot "main.cpp")
target_link_libraries(metabot libmetabot)

install(TARGETS metabot DESTINATION bin)
