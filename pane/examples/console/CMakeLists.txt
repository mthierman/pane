add_executable(console)
target_sources(
    console
    PRIVATE "main.cxx"
            "../../data/app.manifest"
    )
target_link_libraries(
    console
    PRIVATE pane::pane
            pane::config
    )
