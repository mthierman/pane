add_custom_target(
    util_install_wix
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMAND dotnet tool install --global wix
    COMMAND wix extension add WixToolset.BootstrapperApplications.wixext
    COMMAND wix extension add WixToolset.UI.wixext
)
