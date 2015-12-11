cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

set (BASE
    "${CMAKE_SOURCE_DIR}/..")

set (METABOT_DIR
    "${BASE}/metabot/" CACHE STRING "Metabot directory")

set (RHIO_DIR
    "${BASE}/../RhIO")

set (CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wno-reorder -pedantic -fno-elide-constructors")

include_directories("${METABOT_DIR}")
include_directories("${METABOT_DIR}/sym")
include_directories("${BULLET_DIR}/src")
add_subdirectory(${METABOT_DIR} metabot)
include_directories(${BULLET_INCLUDE_DIRS})
add_subdirectory(${RHIO_DIR} rhio)
include_directories("${RHIO_DIR}/include")

set (SOURCES
    Controller.cpp
    Function.cpp
    Simulation.cpp

    util.cpp
    kinematic.cpp
    main.cpp
    verbose.cpp
    )

add_executable(sim ${SOURCES})
target_link_libraries(sim pthread libmetabot RhIO)
