###########################################################
## TARGETS
###########################################################
aux_source_directory(./ P010TONV12_SRC_FILES)
add_executable(p010tonv12 ${P010TONV12_SRC_FILES})

target_link_libraries(p010tonv12 cam-tools-common)
