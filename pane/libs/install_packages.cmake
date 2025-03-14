add_custom_target(
    ${PROJECT_NAME}_install_packages WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMAND pwsh -nol -nop -f "${PROJECT_NAME}/scripts/install.ps1"
    )
