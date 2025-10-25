FetchContent_Declare(
    webview2
    DOWNLOAD_NO_PROGRESS ON
    URL
        https://www.nuget.org/api/v2/package/Microsoft.Web.WebView2/${PANE_WEBVIEW2_VERSION}
        SOURCE_SUBDIR
        NULL
)

FetchContent_MakeAvailable(webview2)

add_library(webview2 INTERFACE)

add_library(microsoft::webview2 ALIAS webview2)

target_include_directories(webview2 SYSTEM INTERFACE ${webview2_SOURCE_DIR}/build/native/include)

target_link_directories(webview2 INTERFACE ${webview2_SOURCE_DIR}/build/native/x64)

target_link_libraries(webview2 INTERFACE WebView2LoaderStatic)
