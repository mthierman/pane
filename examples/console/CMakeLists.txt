add_executable(${PROJECT_NAME}_example_console)

target_sources(${PROJECT_NAME}_example_console PRIVATE "console.cxx")

target_link_libraries(
    ${PROJECT_NAME}_example_console
    PRIVATE ${PROJECT_NAME}::${PROJECT_NAME}
            ${PROJECT_NAME}::config
    )
