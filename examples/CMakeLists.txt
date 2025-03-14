add_executable(${PROJECT_NAME}_examples_console)

target_sources(${PROJECT_NAME}_examples_console PRIVATE "src/console.cxx")

target_link_libraries(
    ${PROJECT_NAME}_examples_console
    PRIVATE ${PROJECT_NAME}::${PROJECT_NAME}
            ${PROJECT_NAME}::config
    )

add_executable(${PROJECT_NAME}_examples_json)

target_sources(${PROJECT_NAME}_examples_json PRIVATE "src/json.cxx")

target_link_libraries(
    ${PROJECT_NAME}_examples_json
    PRIVATE ${PROJECT_NAME}::${PROJECT_NAME}
            ${PROJECT_NAME}::config
            ${PROJECT_NAME}::glaze
    )

add_executable(
    ${PROJECT_NAME}_examples_window
    WIN32
    )

target_sources(${PROJECT_NAME}_examples_window PRIVATE "src/window.cxx")

target_link_libraries(
    ${PROJECT_NAME}_examples_window
    PRIVATE ${PROJECT_NAME}::${PROJECT_NAME}
            ${PROJECT_NAME}::config
    )
