add_executable(example_console)

target_sources(example_console PRIVATE "src/main.cxx")

target_link_libraries(
    example_console
    PRIVATE pane::pane
            pane::flags
    )
