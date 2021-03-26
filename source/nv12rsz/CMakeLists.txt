###########################################################
## TARGETS
###########################################################
aux_source_directory(./ NV12RSZ_SRC_FILES)
add_executable(nv12rsz ${NV12RSZ_SRC_FILES})

target_link_libraries(nv12rsz cam-tools-common)
