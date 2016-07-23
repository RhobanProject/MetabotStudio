cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
find_package(Eigen3 REQUIRED)
find_package(Deps)

deps_add_libraries ()

option (ENABLE_LEGGED "Enable legged experiments" ON)

option (ENABLE_HUMANOID "Enable humanoid experiments" OFF)

set (BASE
    "${CMAKE_SOURCE_DIR}/..")

set (METABOT_DIR
    "${BASE}/metabot/" CACHE STRING "Metabot directory")

set (CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wno-reorder -pedantic -fno-elide-constructors -fopenmp")

include_directories("${METABOT_DIR}")
add_subdirectory(${METABOT_DIR} metabot)
include_directories("${PROJECT_SOURCE_DIR}")

if (USE_DOUBLE_PRECISION)
add_definitions( -DBT_USE_DOUBLE_PRECISION)
endif (USE_DOUBLE_PRECISION)

set (SOURCES
    Simulation.cpp

    Experiment.cpp
    ExperimentTest.cpp
    Generator.cpp

    experiments.cpp
    util.cpp
    main.cpp
    verbose.cpp
    )

if (ENABLE_LEGGED)
    add_definitions(-DENABLE_LEGGED)
    set (SOURCES ${SOURCES}
        legged/Controller.cpp
        legged/ExperimentController.cpp
        legged/ExperimentWalk.cpp
        legged/ExperimentCheckpoints.cpp
        legged/ExperimentAnalyze.cpp
    )
endif ()

if (ENABLE_HUMANOID)
    add_definitions(-DENABLE_HUMANOID)
    deps_add_library("rhobanproject/model")
    set (SOURCES ${SOURCES}
        humanoid/ExperimentStandUp.cpp
        humanoid/ExperimentShoot.cpp
        humanoid/ExperimentStaticShoot.cpp
        humanoid/ExperimentIKWalk.cpp
    )
endif ()

add_executable(sim ${SOURCES})
target_link_libraries(sim pthread libmetabot ${DEPS_LIBRARIES})

