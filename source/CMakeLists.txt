cmake_minimum_required(VERSION 2.8.9)
project (cppdwt)

# find_package(Curses REQUIRED)
# include_directories(${CURSES_INCLUDE_DIR})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")

aux_source_directory(. DWT_SRC_FILES)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_executable(cppdwt ${DWT_SRC_FILES})

