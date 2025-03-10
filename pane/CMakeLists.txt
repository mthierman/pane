add_library(${PROJECT_NAME})

add_library(
    pane::pane
    ALIAS
    ${PROJECT_NAME}
    )

target_sources(
    ${PROJECT_NAME}
    PRIVATE "filesystem.cxx"
            "hstring.cxx"
            "string.cxx"
            "system.cxx"
            "text.cxx"
            "window.cxx"
    PUBLIC FILE_SET
           HEADERS
           BASE_DIRS
           "include"
    )

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE ${PROJECT_NAME}_config
            icu
            # ada::ada
            glaze::glaze
            microsoft::webview2
            microsoft::wil
            microsoft::cppwinrt
    )
