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

add_subdirectory("pane")

if(PROJECT_IS_TOP_LEVEL)
    add_subdirectory("examples")
    add_subdirectory("task")
    add_subdirectory("browser")
endif()
