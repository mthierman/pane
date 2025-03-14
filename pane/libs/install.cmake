add_custom_target(
    ${PROJECT_NAME}_install WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMAND pwsh -nol -nop -f "${CMAKE_SOURCE_DIR}/scripts/install.ps1"
    )
