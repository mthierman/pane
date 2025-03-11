add_executable(
    ${PROJECT_NAME}_example_window
    WIN32
    )

target_sources(${PROJECT_NAME}_example_window PRIVATE "window.cxx")

target_link_libraries(
    ${PROJECT_NAME}_example_window
    PRIVATE ${PROJECT_NAME}::${PROJECT_NAME}
            ${PROJECT_NAME}::config
    )
