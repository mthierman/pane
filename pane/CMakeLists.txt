include("libs/config")
include("libs/webview2")
include("libs/wil")
include("libs/cppwinrt")
include("libs/ada")
include("libs/glaze")

add_custom_target(
    ${PROJECT_NAME}_install WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMAND pwsh -nol -nop -f "${CMAKE_SOURCE_DIR}/scripts/install.ps1"
    )

add_library(${PROJECT_NAME})

add_library(
    ${PROJECT_NAME}::${PROJECT_NAME}
    ALIAS
    ${PROJECT_NAME}
    )

target_sources(
    ${PROJECT_NAME}
    PUBLIC FILE_SET
           ${PROJECT_NAME}_headers
           TYPE
           HEADERS
           BASE_DIRS
           "include"
           FILES
           "include/pane/pane.hxx"
           "include/pane/filesystem.hxx"
           "include/pane/hstring.hxx"
           "include/pane/string.hxx"
           "include/pane/system.hxx"
           "include/pane/text.hxx"
           "include/pane/window.hxx"
    PRIVATE "src/filesystem.cxx"
            "src/hstring.cxx"
            "src/string.cxx"
            "src/system.cxx"
            "src/text.cxx"
            "src/window.cxx"
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

install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}_targets
    FILE_SET
    ${PROJECT_NAME}_headers
    )

install(TARGETS ${PROJECT_NAME}_config EXPORT ${PROJECT_NAME}_targets)

install(TARGETS ${PROJECT_NAME}_ada EXPORT ${PROJECT_NAME}_targets)
install(TARGETS ${PROJECT_NAME}_glaze EXPORT ${PROJECT_NAME}_targets)
install(TARGETS ${PROJECT_NAME}_webview2 EXPORT ${PROJECT_NAME}_targets)
install(TARGETS ${PROJECT_NAME}_cppwinrt EXPORT ${PROJECT_NAME}_targets)
install(TARGETS ${PROJECT_NAME}_wil EXPORT ${PROJECT_NAME}_targets)

install(EXPORT ${PROJECT_NAME}_targets DESTINATION "share" NAMESPACE ${PROJECT_NAME}::)

if(PROJECT_IS_TOP_LEVEL)
    add_executable(${PROJECT_NAME}_example_console)

    target_sources(${PROJECT_NAME}_example_console PRIVATE "example/console.cxx")

    target_link_libraries(
        ${PROJECT_NAME}_example_console
        PRIVATE ${PROJECT_NAME}::${PROJECT_NAME}
                ${PROJECT_NAME}::config
        )

    add_executable(${PROJECT_NAME}_example_json)

    target_sources(${PROJECT_NAME}_example_json PRIVATE "example/json.cxx")

    target_link_libraries(
        ${PROJECT_NAME}_example_json
        PRIVATE ${PROJECT_NAME}::${PROJECT_NAME}
                ${PROJECT_NAME}::config
                ${PROJECT_NAME}::glaze
        )

    add_executable(
        ${PROJECT_NAME}_example_window
        WIN32
        )

    target_sources(${PROJECT_NAME}_example_window PRIVATE "example/window.cxx")

    target_link_libraries(
        ${PROJECT_NAME}_example_window
        PRIVATE ${PROJECT_NAME}::${PROJECT_NAME}
                ${PROJECT_NAME}::config
        )
endif()
