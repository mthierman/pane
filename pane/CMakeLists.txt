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
    PRIVATE "filesystem.cxx"
            "hstring.cxx"
            "string.cxx"
            "system.cxx"
            "text.cxx"
            "window.cxx"
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
