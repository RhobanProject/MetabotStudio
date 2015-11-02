cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

find_package(gazebo REQUIRED)

set (BASE
    "${CMAKE_SOURCE_DIR}/..")

set (METABOT_DIR
    "${BASE}/metabot/" CACHE STRING "Metabot directory")

include_directories("${METABOT_DIR}")
include_directories("${METABOT_DIR}/sym")
add_subdirectory(${METABOT_DIR} metabot)

include_directories(${GAZEBO_INCLUDE_DIRS})
link_directories(${GAZEBO_LIBRARY_DIRS})
list(APPEND CMAKE_CXX_FLAGS "${GAZEBO_CXX_FLAGS}")

set (SOURCES
    GazeboRobot.cpp
    Controller.cpp
    Function.cpp

    kinematic.cpp
    main.cpp
    )

add_executable(sim ${SOURCES})
target_link_libraries(sim ${GAZEBO_LIBRARIES} pthread libmetabot)
