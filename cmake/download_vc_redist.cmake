add_custom_target(
    util_download_vc_redist
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMAND
        ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/download_vc_redist/download_vc_redist.cmake
)
