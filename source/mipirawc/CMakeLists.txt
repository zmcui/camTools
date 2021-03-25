###########################################################
## TARGETS
###########################################################
aux_source_directory(./ MIPIRAW_SRC_FILES)
add_executable(mipirawc ${MIPIRAW_SRC_FILES})
