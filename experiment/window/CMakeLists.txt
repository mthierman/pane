add_executable(
    experiment_window
    WIN32
    )
target_sources(
    experiment_window
    PRIVATE "main.cxx"
            "../../data/app.manifest"
    )
target_link_libraries(
    experiment_window
    PRIVATE pane::pane
            pane::config
    )
