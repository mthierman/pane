include(FetchContent)

FetchContent_Declare(
    webview2
    DOWNLOAD_NO_PROGRESS ON
    URL "https://www.nuget.org/api/v2/package/Microsoft.Web.WebView2/1.0.3124.44"
        SOURCE_SUBDIR
        "NULL"
    )

FetchContent_MakeAvailable(webview2)

add_library(
    webview2
    INTERFACE
    )

target_include_directories(
    webview2
    SYSTEM
    INTERFACE "${webview2_SOURCE_DIR}/build/native/include"
    )

target_link_directories(
    webview2
    INTERFACE
    "${webview2_SOURCE_DIR}/build/native/x64"
    )

target_link_libraries(webview2 INTERFACE WebView2LoaderStatic)
