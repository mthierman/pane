add_executable(
    webview
    WIN32
    )
target_sources(
    webview
    PRIVATE "main.cxx"
            "../../data/app.manifest"
    )
target_link_libraries(
    webview
    PRIVATE pane::pane
            pane::config
    )
