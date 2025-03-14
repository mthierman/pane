cmake_minimum_required(VERSION 3.30)

list(
    APPEND
    CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
    )

project("pane")

message(STATUS ${CMAKE_MODULE_PATH})

add_subdirectory(${PROJECT_NAME})
