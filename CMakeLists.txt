cmake_minimum_required(VERSION 3.30)

project(pane)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/libs")

find_package(Config)
find_package(WIL)
find_package(CppWinRT)
find_package(WebView2)
find_package(Ada)
find_package(Glaze)

add_subdirectory("pane")

if(PROJECT_IS_TOP_LEVEL)
    add_subdirectory("examples")
    add_subdirectory("task")
    add_subdirectory("browser")
endif()
