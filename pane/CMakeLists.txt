list(
    APPEND
    CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/libs"
    )

include("config")
include("webview2")
include("wil")
include("cppwinrt")
include("ada")
include("glaze")
include("package_install")

add_library(${PROJECT_NAME})

add_library(
    ${PROJECT_NAME}::${PROJECT_NAME}
    ALIAS
    ${PROJECT_NAME}
    )

target_sources(
    ${PROJECT_NAME}
    PUBLIC FILE_SET
           HEADERS
           BASE_DIRS
           "include"
    PRIVATE "src/filesystem.cxx"
            "src/hstring.cxx"
            "src/string.cxx"
            "src/system.cxx"
            "src/text.cxx"
            "src/window.cxx"
    )

target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC ${PROJECT_NAME}::config
           ${PROJECT_NAME}::ada
           ${PROJECT_NAME}::glaze
           ${PROJECT_NAME}::webview2
           ${PROJECT_NAME}::wil
           ${PROJECT_NAME}::cppwinrt
           icu
           Urlmon
    )

add_subdirectory("examples")
