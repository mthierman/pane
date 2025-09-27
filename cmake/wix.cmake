find_program(WIX_EXECUTABLE wix NO_CACHE)

add_custom_target(
    install_wix
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMAND dotnet tool install --global wix
    COMMAND wix extension add WixToolset.BootstrapperApplications.wixext
    COMMAND wix extension add WixToolset.UI.wixext
)

if(NOT TARGET wix)
    add_executable(wix IMPORTED GLOBAL)
    set_target_properties(wix PROPERTIES IMPORTED_LOCATION "${WIX_EXECUTABLE}")
endif()

# if(IS_EXECUTABLE "${WIX_EXECUTABLE}")
#     if(NOT TARGET wix)
#         add_executable(wix IMPORTED GLOBAL)
#         set_target_properties(
#             wix
#             PROPERTIES IMPORTED_LOCATION "${WIX_EXECUTABLE}"
#         )
#         add_dependencies(wix wix_install wix_add_extensions)
#     endif()
# endif()
