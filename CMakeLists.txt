cmake_minimum_required(VERSION 3.31)

project(pane VERSION 0.0.0 DESCRIPTION "C++ library for Windows")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
list(REMOVE_DUPLICATES CMAKE_MODULE_PATH)

if(NOT PROJECT_IS_TOP_LEVEL)
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} PARENT_SCOPE)
endif()

add_subdirectory(pane)

if(PROJECT_IS_TOP_LEVEL)
    if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
        set_property(CACHE CMAKE_INSTALL_PREFIX PROPERTY VALUE "C:/Programs")
    endif()

    add_subdirectory(apps)
    add_subdirectory(examples)
endif()
