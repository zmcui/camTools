cmake_minimum_required(VERSION 2.8.9)
project (camTools)

# find_package(Curses REQUIRED)
# include_directories(${CURSES_INCLUDE_DIR})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

aux_source_directory(./dwtGen DWT_SRC_FILES)
add_executable(cppdwt ${DWT_SRC_FILES})

aux_source_directory(./rawGen RAW_SRC_FILES)
add_executable(rawGen ${RAW_SRC_FILES})

aux_source_directory(./nv12rsz NV12RSZ_SRC_FILES)
add_executable(nv12rsz ${NV12RSZ_SRC_FILES})
