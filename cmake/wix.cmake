find_program(DOTNET_EXECUTABLE dotnet NO_CACHE)

if(IS_EXECUTABLE "${DOTNET_EXECUTABLE}")
    if(NOT TARGET dotnet)
        add_executable(dotnet IMPORTED GLOBAL)
        set_target_properties(
            dotnet
            PROPERTIES IMPORTED_LOCATION "${DOTNET_EXECUTABLE}"
        )
        add_custom_target(
            wix
            COMMAND dotnet tool restore
            COMMAND
                dotnet wix extension add
                WixToolset.BootstrapperApplications.wixext
            COMMAND dotnet wix extension add WixToolset.UI.wixext
        )
    endif()
endif()
