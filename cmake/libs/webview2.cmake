include(FetchContent)

FetchContent_Declare(
    ${PROJECT_NAME}_webview2
    DOWNLOAD_NO_PROGRESS ON
    URL "https://www.nuget.org/api/v2/package/Microsoft.Web.WebView2/1.0.3124.44"
        SOURCE_SUBDIR
        "NULL"
    )

FetchContent_MakeAvailable(${PROJECT_NAME}_webview2)

add_library(
    ${PROJECT_NAME}_webview2
    INTERFACE
    )

add_library(
    ${PROJECT_NAME}::webview2
    ALIAS
    ${PROJECT_NAME}_webview2
    )

target_include_directories(
    ${PROJECT_NAME}_webview2
    SYSTEM
    INTERFACE "${${PROJECT_NAME}_webview2_SOURCE_DIR}/build/native/include"
    )

target_link_directories(
    ${PROJECT_NAME}_webview2
    INTERFACE
    "${${PROJECT_NAME}_webview2_SOURCE_DIR}/build/native/x64"
    )

target_link_libraries(${PROJECT_NAME}_webview2 INTERFACE WebView2LoaderStatic)
