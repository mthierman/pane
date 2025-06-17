add_executable(
    webview_docs
    WIN32
    )
target_sources(
    webview_docs
    PRIVATE "main.cxx"
            "../data/app.manifest"
    )
target_link_libraries(
    webview_docs
    PRIVATE pane::pane
            pane::config
    )
