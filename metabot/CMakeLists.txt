cmake_minimum_required(VERSION 2.8)
project(metabot)
find_package (Threads)

option (OPENGL "Enable OpenGL" OFF)

if (OPENGL)
    add_definitions(-DOPENGL)
endif()

if (WIN32)
	add_definitions(-DWIN32)
endif()

set (CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wno-reorder -pedantic")

set(ALL_SOURCES
    util/Cache.cpp
    util/sha1.cpp
    util/util.cpp

    3d/stl.cpp
    3d/Cube.cpp
    3d/Face.cpp
    3d/Line.cpp
    3d/Model.cpp
    3d/Octree.cpp
    3d/Point3.cpp
    3d/Volume.cpp
    
    scad/BOM.cpp
    scad/CSG.cpp
    scad/CSGNode.cpp
    scad/Module.cpp
    scad/SCAD.cpp
    scad/Shape.cpp
    
    AnchorPoint.cpp
    Backend.cpp
    Component.cpp
    Dynamics.cpp
    Parameter.cpp
    Parameters.cpp
    Ref.cpp
    Robot.cpp
    TransformMatrix.cpp
    Values.cpp
    Vector.cpp
    Voxels.cpp
    )

include_directories("${CMAKE_SOURCE_DIR}")
include_directories("${CMAKE_SOURCE_DIR}/util/")
include_directories("${CMAKE_SOURCE_DIR}/scad/")
include_directories("${CMAKE_SOURCE_DIR}/sym/")
include_directories("${CMAKE_SOURCE_DIR}/json/include")
add_subdirectory("${CMAKE_SOURCE_DIR}/json")

add_library(symbolic sym/symbolicc++.cpp)
set_target_properties(symbolic PROPERTIES COMPILE_DEFINITIONS "LIBSYMBOLICCPLUSPLUS")

add_library(libmetabot ${ALL_SOURCES})

target_link_libraries(libmetabot ${CMAKE_THREAD_LIBS_INIT} json symbolic)
if (OPENGL)
    target_link_libraries(libmetabot GL GLU)
endif ()

add_executable(metabot "main.cpp")
target_link_libraries(metabot libmetabot)

install(TARGETS metabot DESTINATION bin)
