###########################################################
## TARGETS
###########################################################
aux_source_directory(./ MIPIRAW_SRC_FILES)
add_executable(mipirawc ${MIPIRAW_SRC_FILES})

target_link_libraries(mipirawc cam-tools-common)
install(TARGETS mipirawc RUNTIME DESTINATION bin)
