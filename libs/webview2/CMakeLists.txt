include(FetchContent)

FetchContent_Declare(
    pane_webview2
    URL "https://www.nuget.org/api/v2/package/Microsoft.Web.WebView2/1.0.3124.44"
        SOURCE_SUBDIR
        "NULL"
    )

FetchContent_MakeAvailable(pane_webview2)

add_library(
    pane_webview2
    INTERFACE
    )

add_library(
    pane::webview2
    ALIAS
    pane_webview2
    )

target_include_directories(
    pane_webview2
    SYSTEM
    INTERFACE "${pane_webview2_SOURCE_DIR}/build/native/include"
    )

target_link_directories(
    pane_webview2
    INTERFACE
    "${pane_webview2_SOURCE_DIR}/build/native/x64"
    )

target_link_libraries(pane_webview2 INTERFACE WebView2LoaderStatic)
