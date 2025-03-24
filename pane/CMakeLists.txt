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
    PUBLIC ${PROJECT_NAME}::ada
           ${PROJECT_NAME}::glaze
           ${PROJECT_NAME}::webview2
           ${PROJECT_NAME}::wil
           ${PROJECT_NAME}::cppwinrt
    PRIVATE ${PROJECT_NAME}::config
            icu
            Urlmon
    )

if(PROJECT_IS_TOP_LEVEL)
    add_subdirectory("examples")
endif()
