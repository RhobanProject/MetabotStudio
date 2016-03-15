cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
find_package(Deps)

deps_add_library ("rhobandeps/jsoncpp")
deps_add_library ("rhobandeps/bullet3")
deps_add_library ("rhobandeps/libcmaes")

set (BASE
    "${CMAKE_SOURCE_DIR}/..")

set (METABOT_DIR
    "${BASE}/metabot/" CACHE STRING "Metabot directory")

set (CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wno-reorder -pedantic -fno-elide-constructors -fopenmp")

include_directories("${METABOT_DIR}")
include_directories("${METABOT_DIR}/sym")
include_directories("${METABOT_DIR}/json/include")
include_directories("${BULLET_DIR}/src")
add_subdirectory(${METABOT_DIR} metabot)
include_directories("${BULLET_DIR}/src")
include_directories(${BULLET_INCLUDE_DIRS})

set (EIGEN3_INCLUDE_DIR
    "/net/i/gpassault/notroot/usr/include/eigen3")
include_directories("${EIGEN3_INCLUDE_DIR}")

if (USE_DOUBLE_PRECISION)
add_definitions( -DBT_USE_DOUBLE_PRECISION)
endif (USE_DOUBLE_PRECISION)

set (SOURCES
    Simulation.cpp
    Experience.cpp
    ExperienceWalk.cpp
    ExperienceTest.cpp

    util.cpp
    main.cpp
    verbose.cpp
    )

add_executable(sim ${SOURCES})
target_link_libraries(sim pthread libmetabot ${DEPS_LIBRARIES})

execute_process (COMMAND "${CMAKE_SOURCE_DIR}/makeln.sh" 
    "${CMAKE_BINARY_DIR}/backends"
    "${BASE}/backends"
    )

execute_process (COMMAND "${CMAKE_SOURCE_DIR}/makeln.sh" 
    "${CMAKE_BINARY_DIR}/viewer"
    "${BASE}/gui/Metabot"
    c/METABOT_
    )
