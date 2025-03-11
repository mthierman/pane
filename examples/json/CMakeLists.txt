add_executable(${PROJECT_NAME}_example_json)

target_sources(${PROJECT_NAME}_example_json PRIVATE "json.cxx")

target_link_libraries(
    ${PROJECT_NAME}_example_json
    PRIVATE ${PROJECT_NAME}::${PROJECT_NAME}
            ${PROJECT_NAME}::config
            ${PROJECT_NAME}::glaze
    )
