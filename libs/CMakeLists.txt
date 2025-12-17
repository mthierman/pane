include(FetchContent)

# https://github.com/ada-url/ada
# https://github.com/ada-url/ada/releases
set(PANE_ADA_VERSION "3.3.0")
add_subdirectory(ada)

# https://github.com/stephenberry/argz
# https://github.com/stephenberry/argz/releases
set(PANE_ARGZ_VERSION "0.2.1")
add_subdirectory(argz)

# https://www.nuget.org/packages/Microsoft.Windows.CppWinRT
set(PANE_CPPWINRT_VERSION "2.0.250303.1")
add_subdirectory(cppwinrt)

# https://github.com/stephenberry/glaze
# https://github.com/stephenberry/glaze/releases
set(PANE_GLAZE_VERSION "6.2.0")
add_subdirectory(glaze)

# https://sqlite.org/
# https://sqlite.org/download.html
set(PANE_SQLITE_VERSION "3510100")
add_subdirectory(sqlite)

# https://www.nuget.org/packages/Microsoft.Web.WebView2
# https://learn.microsoft.com/en-us/microsoft-edge/webview2/landing/
set(PANE_WEBVIEW2_VERSION "1.0.3650.58")
add_subdirectory(webview2)

# https://www.nuget.org/packages/Microsoft.Windows.ImplementationLibrary
set(PANE_WIL_VERSION "1.0.250325.1")
add_subdirectory(wil)
