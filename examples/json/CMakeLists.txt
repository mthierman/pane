add_executable(example_json)

target_sources(example_json PRIVATE "src/main.cxx")

target_link_libraries(
    example_json
    PRIVATE pane::pane
            pane::config
            pane::glaze
    )
