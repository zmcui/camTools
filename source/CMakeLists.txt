include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")

###########################################################
## SUBDIRECTORIES
###########################################################

add_subdirectory(common)

add_subdirectory(dwtGen)
add_subdirectory(rawGen)
add_subdirectory(nv12rsz)
add_subdirectory(mipirawc)
add_subdirectory(p010tonv12)
