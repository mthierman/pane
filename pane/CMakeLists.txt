add_library(${PROJECT_NAME})

add_library(
    pane::pane
    ALIAS
    ${PROJECT_NAME}
    )

target_sources(
    ${PROJECT_NAME}
    PUBLIC FILE_SET
           HEADERS
           BASE_DIRS
           "include"
    PRIVATE "filesystem.cxx"
            "hstring.cxx"
            "string.cxx"
            "system.cxx"
            "text.cxx"
            "window.cxx"
    )

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE ${PROJECT_NAME}_config
            icu
            pane::ada
            pane::glaze
            pane::webview2
            pane::wil
            pane::cppwinrt
    )
