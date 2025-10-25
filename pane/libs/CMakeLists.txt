include(FetchContent)

# https://github.com/ada-url/ada
# https://github.com/ada-url/ada/releases
set(ADA_VERSION "3.3.0" CACHE STRING "Ada version")
add_subdirectory(ada)

# https://github.com/stephenberry/argz
# https://github.com/stephenberry/argz/releases
set(ARGZ_VERSION "0.2.1" CACHE STRING "Argz version")
add_subdirectory(argz)

# https://www.nuget.org/packages/Microsoft.Windows.CppWinRT
set(CPPWINRT_VERSION "2.0.250303.1" CACHE STRING "C++/WinRT version")
add_subdirectory(cppwinrt)

# https://github.com/stephenberry/glaze
# https://github.com/stephenberry/glaze/releases
set(GLAZE_VERSION "6.0.1" CACHE STRING "Glaze version")
add_subdirectory(glaze)

# https://sqlite.org/
# https://sqlite.org/download.html
set(SQLITE_VERSION "3500400" CACHE STRING "SQLite version")
add_subdirectory(sqlite)

# https://www.nuget.org/packages/Microsoft.Web.WebView2
set(WEBVIEW2_VERSION "1.0.3537.50" CACHE STRING "WebView2 version")
add_subdirectory(webview2)

# https://www.nuget.org/packages/Microsoft.Windows.ImplementationLibrary
set(WIL_VERSION "1.0.250325.1" CACHE STRING "WIL Version")
add_subdirectory(wil)
