cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
find_package (Bullet)

set (BASE
    "${CMAKE_SOURCE_DIR}/..")

set (METABOT_DIR
    "${BASE}/metabot/" CACHE STRING "Metabot directory")

set (CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wno-reorder -pedantic -fno-elide-constructors")

include_directories("${METABOT_DIR}")
include_directories("${METABOT_DIR}/sym")
add_subdirectory(${METABOT_DIR} metabot)
include_directories(${BULLET_INCLUDE_DIRS})

set (SOURCES
    Controller.cpp
    Function.cpp

    util.cpp
    kinematic.cpp
    main.cpp
    verbose.cpp
    )

add_executable(sim ${SOURCES})
target_link_libraries(sim pthread libmetabot)
