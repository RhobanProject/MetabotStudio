cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
find_package(Eigen3 REQUIRED)

set (BASE
    "${CMAKE_SOURCE_DIR}/..")

set (METABOT_DIR
    "${BASE}/metabot/" CACHE STRING "Metabot directory")

set (CMAES_DIR
    "${BASE}/libcmaes" CACHE STRING "cmaes directory")

set (CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wno-reorder -pedantic -fno-elide-constructors")

include_directories("${METABOT_DIR}")
include_directories("${METABOT_DIR}/sym")
include_directories("${METABOT_DIR}/json/include")
include_directories("${BULLET_DIR}/src")
add_subdirectory(${METABOT_DIR} metabot)
include_directories("${BULLET_DIR}/src")
include_directories(${BULLET_INCLUDE_DIRS})
include_directories("${CMAES_DIR}/src")
include_directories("${CMAES_DIR}/")
add_subdirectory(${CMAES_DIR} cmaes)
include_directories("${EIGEN3_INCLUDE_DIR}")

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
target_link_libraries(sim pthread libmetabot cmaes gomp)
