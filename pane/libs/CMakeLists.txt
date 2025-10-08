include(FetchContent)

set(ADA_VERSION "3.3.0" CACHE STRING "Ada version")
add_subdirectory(ada)

set(ARGZ_VERSION "0.2.1" CACHE STRING "Argz version")
add_subdirectory(argz)

set(CPPWINRT_VERSION "2.0.250303.1" CACHE STRING "C++/WinRT version")
add_subdirectory(cppwinrt)

set(GLAZE_VERSION "5.7.2" CACHE STRING "Glaze version")
add_subdirectory(glaze)

set(SQLITE_VERSION "3500400" CACHE STRING "SQLite version")
add_subdirectory(sqlite)

set(WEBVIEW2_VERSION "1.0.3485.44" CACHE STRING "WebView2 version")
add_subdirectory(webview2)

set(WIL_VERSION "1.0.250325.1" CACHE STRING "WIL Version")
add_subdirectory(wil)
