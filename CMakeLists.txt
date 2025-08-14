cmake_minimum_required(VERSION 3.30)

project(pane)

list(
    APPEND
    CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
    )

include(FetchContent)
include(libs/config)
include(libs/webview2)
include(libs/wil)
include(libs/cppwinrt)
include(libs/ada)
include(libs/glaze)

add_library(pane)
add_library(
    pane::pane
    ALIAS
    pane
    )
target_sources(
    pane
    PUBLIC FILE_SET
           HEADERS
           BASE_DIRS
           "include"
    PRIVATE "src/co_init.cpp"
            "src/color.cpp"
            "src/config.cpp"
            "src/console.cpp"
            "src/debug.cpp"
            "src/error.cpp"
            "src/file_picker.cpp"
            "src/filesystem.cpp"
            "src/gdi_plus.cpp"
            "src/guid.cpp"
            "src/hstring.cpp"
            "src/icu.cpp"
            "src/input.cpp"
            "src/math.cpp"
            "src/message_box.cpp"
            "src/process.cpp"
            "src/string.cpp"
            "src/system.cpp"
            "src/text.cpp"
            "src/url.cpp"
            "src/webview.cpp"
            "src/window.cpp"
    )
target_link_libraries(
    pane
    PRIVATE pane::config
    PUBLIC dwmapi
           gdiplus
           Urlmon
           WindowsApp
           ada
           glaze
           webview2
           wil
           cppwinrt
    )

if(PROJECT_IS_TOP_LEVEL)
    add_subdirectory("examples")
    add_subdirectory("task")
    add_subdirectory("browser")
endif()
