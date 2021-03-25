###########################################################
## TARGETS
###########################################################
aux_source_directory(./ DWT_SRC_FILES)
add_executable(cppdwt ${DWT_SRC_FILES})
