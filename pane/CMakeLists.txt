include("config")

include("libs/ada")
include("libs/cppwinrt")
include("libs/glaze")
include("libs/webview2")
include("libs/wil")

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
    PRIVATE "src/pane/filesystem.cxx"
            "src/pane/hstring.cxx"
            "src/pane/string.cxx"
            "src/pane/system.cxx"
            "src/pane/text.cxx"
            "src/pane/window.cxx"
    )

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE ${PROJECT_NAME}::config
            ${PROJECT_NAME}::ada
            ${PROJECT_NAME}::glaze
            ${PROJECT_NAME}::webview2
            ${PROJECT_NAME}::wil
            ${PROJECT_NAME}::cppwinrt
            icu
    )
