add_executable(
    window
    WIN32
    )
target_sources(
    window
    PRIVATE "main.cxx"
            "../../data/app.manifest"
    )
target_link_libraries(
    window
    PRIVATE pane::pane
            pane::config
    )
