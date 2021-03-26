####################################
cmake_minimum_required(VERSION 2.8.7)
project(camTools)

EXECUTE_PROCESS(COMMAND git describe --tags
    TIMEOUT 5
    OUTPUT_VARIABLE GIT_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
MESSAGE(STATUS "building from git tag ${GIT_VERSION}")
add_definitions(-DBUILD_VERSION=\"${GIT_VERSION}\")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(source)
