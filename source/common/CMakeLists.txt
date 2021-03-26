###########################################################
## SOURCES
###########################################################
set(CAM_TOOLS_COMMON_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/version.c
)

###########################################################
## TARGETS
###########################################################
add_library(cam-tools-common STATIC ${CAM_TOOLS_COMMON_SOURCES})
# If supported for the target machine, emit position-independent code
target_compile_options(cam-tools-common PUBLIC -fPIC)
