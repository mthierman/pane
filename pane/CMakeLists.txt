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
    PRIVATE "src/co_init.cxx"
            "src/color.cxx"
            "src/file_picker.cxx"
            "src/file.cxx"
            "src/gdi_plus.cxx"
            "src/guid.cxx"
            "src/hstring.cxx"
            "src/icu.cxx"
            "src/string.cxx"
            "src/system.cxx"
            "src/url.cxx"
            "src/window.cxx"
    )

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE ${PROJECT_NAME}::config
            icu
            Urlmon
    PUBLIC ${PROJECT_NAME}::ada
           ${PROJECT_NAME}::glaze
           ${PROJECT_NAME}::webview2
           ${PROJECT_NAME}::wil
           ${PROJECT_NAME}::cppwinrt
    )

if(PROJECT_IS_TOP_LEVEL)
    add_subdirectory("examples")
endif()
