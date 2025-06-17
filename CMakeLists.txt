cmake_minimum_required(VERSION 3.30)

project(pane)

list(
    APPEND
    CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
    )
list(
    APPEND
    CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/libs"
    )

include(FetchContent)
include(config)
include(webview2)
include(wil)
include(cppwinrt)
include(ada)
include(glaze)

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
    PRIVATE "src/co_init.cxx"
            "src/color.cxx"
            "src/config.cxx"
            "src/console.cxx"
            "src/debug.cxx"
            "src/error.cxx"
            "src/file_picker.cxx"
            "src/filesystem.cxx"
            "src/gdi_plus.cxx"
            "src/guid.cxx"
            "src/hstring.cxx"
            "src/icu.cxx"
            "src/input.cxx"
            "src/math.cxx"
            "src/message_box.cxx"
            "src/process.cxx"
            "src/string.cxx"
            "src/system.cxx"
            "src/text.cxx"
            "src/url.cxx"
            "src/window.cxx"
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
    add_subdirectory("webview_docs")
endif()
