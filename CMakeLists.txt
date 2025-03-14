cmake_minimum_required(VERSION 3.30)

project("pane")

list(
    APPEND
    CMAKE_MODULE_PATH
    "${PROJECT_SOURCE_DIR}/cmake"
    )

add_subdirectory("pane")
