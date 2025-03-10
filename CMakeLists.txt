cmake_minimum_required(VERSION 3.30)

project(pane)

list(
    APPEND
    CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
    )

add_subdirectory("libs")
add_subdirectory("pane")

if(PROJECT_IS_TOP_LEVEL)
    add_subdirectory("examples")
endif()
