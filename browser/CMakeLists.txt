add_executable(
    browser
    WIN32
    )
target_sources(
    browser
    PRIVATE "main.cxx"
            "../data/app.manifest"
    )
target_link_libraries(
    browser
    PRIVATE pane::pane
            pane::config
    )
