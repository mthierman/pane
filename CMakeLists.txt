cmake_minimum_required(VERSION 3.30)

project(pane)

list(
    APPEND
    CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
    )

add_subdirectory("libs")

add_library(pane_pane)

add_library(
    pane::pane
    ALIAS
    pane_pane
    )

target_sources(
    pane_pane
    PRIVATE "src/filesystem.cxx"
            "src/hstring.cxx"
            "src/string.cxx"
            "src/system.cxx"
            "src/text.cxx"
            "src/window.cxx"
    PUBLIC FILE_SET
           HEADERS
           BASE_DIRS
           "src/include"
    )

target_compile_features(
    pane_pane
    PUBLIC c_std_17
           cxx_std_23
    )

target_compile_definitions(
    pane_pane
    PUBLIC UNICODE
           WIN32_LEAN_AND_MEAN
           NOMINMAX
           GDIPVER=0x0110
    )

# https://learn.microsoft.com/en-us/cpp/build/reference/compiler-options
target_compile_options(
    pane_pane
    PUBLIC /W4
           /WX
           /MP
           /utf-8
           /bigobj
           /diagnostics:caret
           /permissive-
           /Zc:__cplusplus,__STDC__,enumTypes,templateScope,throwingNew,preprocessor
    )

# https://learn.microsoft.com/en-us/cpp/build/reference/linker-options
target_link_options(
    pane_pane
    PUBLIC
    /WX
    )

target_link_libraries(
    pane_pane
    PUBLIC icu
           ada::ada
           glaze::glaze
           microsoft::webview2
           microsoft::wil
           microsoft::cppwinrt
    )

if(PROJECT_IS_TOP_LEVEL)
    add_subdirectory("examples")
endif()
