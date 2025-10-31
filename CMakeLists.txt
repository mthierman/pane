cmake_minimum_required(VERSION 3.31)
project(pane)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
list(REMOVE_DUPLICATES CMAKE_MODULE_PATH)

if(NOT PROJECT_IS_TOP_LEVEL)
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} PARENT_SCOPE)
endif()

add_subdirectory(libs)

add_library(pane)

add_library(pane::pane ALIAS pane)

target_compile_features(pane PUBLIC c_std_17 cxx_std_23)

target_compile_definitions(pane PUBLIC UNICODE WIN32_LEAN_AND_MEAN NOMINMAX GDIPVER=0x0110)

target_sources(
    pane
    PUBLIC FILE_SET HEADERS BASE_DIRS include ${CMAKE_CURRENT_BINARY_DIR}/generated
    PRIVATE
        src/co_init.cpp
        src/color.cpp
        src/config.cpp
        src/console.cpp
        src/error.cpp
        src/file_picker.cpp
        src/filesystem.cpp
        src/gdi_plus.cpp
        src/guid.cpp
        src/input.cpp
        src/math.cpp
        src/print.cpp
        src/process.cpp
        src/system.cpp
        src/text.cpp
        src/url.cpp
        src/webview.cpp
        src/window.cpp
)

target_link_libraries(
    pane
    PUBLIC
        dwmapi
        gdiplus
        Urlmon
        WindowsApp
        ada::ada-url
        microsoft::cppwinrt
        microsoft::webview2
        microsoft::wil
        sqlite::sqlite
        stephenberry::argz
        stephenberry::glaze
)

target_compile_options(
    pane
    PUBLIC
        $<$<CXX_COMPILER_ID:MSVC>:
        /W4
        /WX
        /utf-8
        /bigobj
        /diagnostics:caret
        /permissive-
        /Zc:__cplusplus,enumTypes,templateScope,throwingNew,preprocessor
        >
        $<$<AND:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_FRONTEND_VARIANT:MSVC>>:
        /W4
        /WX
        -Wno-braced-scalar-init
        >
        $<$<AND:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_FRONTEND_VARIANT:GNU>>:
        -Wall
        -Werror
        -Wno-braced-scalar-init
        >
)

target_link_options(
    pane
    PUBLIC
        $<$<CXX_COMPILER_ID:MSVC>:
        /WX
        >
        $<$<AND:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_FRONTEND_VARIANT:MSVC>>:
        /WX
        >
        $<$<AND:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_FRONTEND_VARIANT:GNU>>:
        -Wl,/WX
        >
)

if(PROJECT_IS_TOP_LEVEL)
    include(download_vc_redist)
    include(install_wix)
    add_subdirectory(apps)
    add_subdirectory(examples)
    add_subdirectory(tests)
endif()
