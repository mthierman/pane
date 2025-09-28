cmake_minimum_required(VERSION 3.31)
project(pane)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
list(REMOVE_DUPLICATES CMAKE_MODULE_PATH)

if(NOT PROJECT_IS_TOP_LEVEL)
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} PARENT_SCOPE)
endif()

add_subdirectory(pane)
add_subdirectory(tasks)

if(PROJECT_IS_TOP_LEVEL)
    add_subdirectory(examples)
    add_subdirectory(task)
    add_subdirectory(browser)
endif()
