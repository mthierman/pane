cmake_minimum_required(VERSION 3.30)

project(pane)

list(
    APPEND
    CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
    )

add_subdirectory("libs")

add_library(
    ${PROJECT_NAME}_flags
    INTERFACE
    )

add_library(
    pane::flags
    ALIAS
    ${PROJECT_NAME}_flags
    )

target_compile_definitions(
    ${PROJECT_NAME}_flags
    INTERFACE UNICODE
              WIN32_LEAN_AND_MEAN
              NOMINMAX
              GDIPVER=0x0110
    )

# https://learn.microsoft.com/en-us/cpp/build/reference/compiler-options
target_compile_options(
    ${PROJECT_NAME}_flags
    INTERFACE /W4
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
    ${PROJECT_NAME}_flags
    INTERFACE
    /WX
    )

add_library(${PROJECT_NAME})

add_library(
    pane::pane
    ALIAS
    ${PROJECT_NAME}
    )

target_sources(
    ${PROJECT_NAME}
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
    ${PROJECT_NAME}
    PUBLIC c_std_17
           cxx_std_23
    )

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE ${PROJECT_NAME}_flags
            icu
            ada::ada
            glaze::glaze
            microsoft::webview2
            microsoft::wil
            microsoft::cppwinrt
    )

if(PROJECT_IS_TOP_LEVEL)
    add_subdirectory("examples")
endif()
