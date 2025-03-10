add_custom_target(
    install_packages
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMAND ${CMAKE_COMMAND} -E rm -rf "packages"
    COMMAND
        nuget install "Microsoft.Web.WebView2" -Version "1.0.3065.39" -x -OutputDirectory "packages"
    COMMAND
        nuget install "Microsoft.Windows.CppWinRT" -Version "2.0.240405.15" -x -OutputDirectory
        "packages"
    COMMAND
        nuget install "Microsoft.Windows.ImplementationLibrary" -Version "1.0.240803.1" -x
        -OutputDirectory "packages"
    COMMAND
        nuget install "${CMAKE_CURRENT_SOURCE_DIR}/packages.config" -x -OutputDirectory
        "${CMAKE_SOURCE_DIR}/packages"
        # COMMAND "${CMAKE_SOURCE_DIR}/packages/Microsoft.Windows.CppWinRT/bin/cppwinrt" -input sdk
        # -output "${CMAKE_BINARY_DIR}/cppwinrt" COMMAND git clone --depth=1 --branch=v4.4.3
        # --recurse-submodules "https://github.com/stephenberry/glaze.git"
        # "${CMAKE_SOURCE_DIR}/packages/glaze"
    COMMAND gh repo clone stephenberry/glaze "${CMAKE_SOURCE_DIR}/packages/glaze"
    )
