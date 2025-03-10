# nuget install "Microsoft.Web.WebView2" -Version "1.0.3065.39" -x -OutputDirectory "packages"

# nuget install "Microsoft.Windows.CppWinRT" -Version "2.0.240405.15" -x -OutputDirectory "packages"

# nuget install "Microsoft.Windows.ImplementationLibrary" -Version "1.0.240803.1" -x -OutputDirectory "packages"

# nuget install "${CMAKE_CURRENT_SOURCE_DIR}/packages.config" -x -OutputDirectory "${CMAKE_SOURCE_DIR}/packages"

# "packages/Microsoft.Windows.CppWinRT/bin/cppwinrt" -input sdk -output "${CMAKE_BINARY_DIR}/cppwinrt"

# gh repo clone stephenberry/glaze "${CMAKE_SOURCE_DIR}/packages/glaze"

# git clone --depth=1 --branch=v4.4.3 --recurse-submodules "https://github.com/stephenberry/glaze.git" "packages/glaze"

