cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
find_package(Eigen3 REQUIRED)
find_package(Deps)

deps_add_library ("rhobandeps/jsoncpp")
deps_add_library ("rhobandeps/bullet3")
deps_add_library ("rhobandeps/libcmaes")

set (BASE
    "${CMAKE_SOURCE_DIR}/..")

set (METABOT_DIR
    "${BASE}/metabot/" CACHE STRING "Metabot directory")

set (MODEL_DIR
    "${BASE}/../Model/" CACHE STRING "Leph model directory")

set (CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wno-reorder -pedantic -fno-elide-constructors -fopenmp")

include_directories("${METABOT_DIR}")
include_directories("${METABOT_DIR}/sym")
include_directories("${METABOT_DIR}/json/include")
include_directories("${BULLET_DIR}/src")
add_subdirectory(${METABOT_DIR} metabot)
include_directories("${BULLET_DIR}/src")
include_directories(${BULLET_INCLUDE_DIRS})
include_directories("${EIGEN3_INCLUDE_DIR}")

# Model
add_subdirectory("${MODEL_DIR}" model)
include_directories(${LEPHCODE_INCLUDE_DIRECTORIES})

if (USE_DOUBLE_PRECISION)
add_definitions( -DBT_USE_DOUBLE_PRECISION)
endif (USE_DOUBLE_PRECISION)

set (SOURCES
    Controller.cpp
    Simulation.cpp
    Experiment.cpp
    ExperimentTest.cpp
    ExperimentController.cpp
    ExperimentWalk.cpp
    ExperimentCheckpoints.cpp
    ExperimentStandUp.cpp
    ExperimentShoot.cpp
    ExperimentStaticShoot.cpp
    ExperimentIKWalk.cpp
    Generator.cpp

    util.cpp
    main.cpp
    verbose.cpp
    )

add_executable(sim ${SOURCES})
target_link_libraries(sim pthread libmetabot ${DEPS_LIBRARIES} LephCode)

