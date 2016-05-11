cmake_minimum_required(VERSION 2.8)
project(metabot)
find_package (Threads)
find_package (Deps)

deps_add_library ("rhobandeps/jsoncpp")
deps_add_library ("rhobandeps/bullet3")
deps_add_library ("rhobandeps/libzmq")
deps_add_library ("rhobandeps/symbolic")

OPTION(USE_DOUBLE_PRECISION "Use double precision" ON)

OPTION(BUILD_UNIT_TESTS "Build Unit Tests" OFF)

option (OPENGL "Enable OpenGL" OFF)

if (OPENGL)
    add_definitions(-DOPENGL)
endif()

if (WIN32)
	add_definitions(-DWIN32)
endif()

set (CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wno-reorder -pedantic -fno-elide-constructors")

set(ALL_SOURCES
    util/Benchmark.cpp
    util/Cache.cpp
    util/sha1.cpp
    util/util.cpp
    util/Bin.cpp

    spline/Spline.cpp
    spline/CubicSpline.cpp
    spline/Polynom.cpp
    spline/NewtonBinomial.cpp
    spline/Combination.cpp
    spline/Function.cpp

    bullet/World.cpp
    bullet/GLDebugDrawer.cpp

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

    com/Server.cpp
    com/Client.cpp
    
    AnchorPoint.cpp
    Backend.cpp
    Component.cpp
    Dynamics.cpp
    Kinematic.cpp
    Parameter.cpp
    Parameters.cpp
    Ref.cpp
    Robot.cpp
    TransformMatrix.cpp
    Vect.cpp
    Voxels.cpp
    )

if (USE_DOUBLE_PRECISION)
add_definitions( -DBT_USE_DOUBLE_PRECISION)
endif (USE_DOUBLE_PRECISION)

include_directories("${PROJECT_SOURCE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/util/")
include_directories("${PROJECT_SOURCE_DIR}/scad/")

add_library(libmetabot ${ALL_SOURCES})

target_link_libraries(libmetabot ${CMAKE_THREAD_LIBS_INIT} 
    ${DEPS_LIBRARIES}
    )

if (OPENGL)
    target_link_libraries(libmetabot GL GLU)
endif ()

add_executable(metabot "main.cpp")
target_link_libraries(metabot libmetabot)

install(TARGETS metabot DESTINATION bin)
