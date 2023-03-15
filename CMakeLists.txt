####################################
cmake_minimum_required(VERSION 3.2.5)
project(camTools)

EXECUTE_PROCESS(COMMAND git describe --tags
    TIMEOUT 5
    OUTPUT_VARIABLE GIT_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
MESSAGE(STATUS "building from git tag ${GIT_VERSION}")
add_definitions(-DBUILD_VERSION=\"${GIT_VERSION}\")

set(CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR})
add_subdirectory(source)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
INSTALL(CODE "execute_process( \
    COMMAND ${CMAKE_COMMAND} -E create_symlink \
    ${PROJECT_BINARY_DIR}/compile_commands.json \
    ${PROJECT_SOURCE_DIR}/compile_commands.json \
    )"
)
