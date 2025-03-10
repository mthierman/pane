add_executable(
    example_window
    WIN32
    )

target_sources(example_window PRIVATE "src/main.cxx")

target_link_libraries(
    example_window
    PRIVATE pane::pane
            pane::config
    )
