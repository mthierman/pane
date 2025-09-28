project(tasks NONE)

add_custom_target(
    install_wix
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMAND dotnet tool install --global wix
    COMMAND wix extension add WixToolset.BootstrapperApplications.wixext
    COMMAND wix extension add WixToolset.UI.wixext
)

add_custom_target(
    install_packages
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMAND pwsh -nol -nop -f ${CMAKE_CURRENT_LIST_DIR}/install_packages.ps1
)

add_custom_target(
    download_vc_redist
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMAND
        ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/download_vc_redist.cmake
)
