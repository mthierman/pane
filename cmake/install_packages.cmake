add_custom_target(
    install_packages WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMAND pwsh -nol -nop -f "${CMAKE_SOURCE_DIR}/scripts/install_packages.ps1"
    )
