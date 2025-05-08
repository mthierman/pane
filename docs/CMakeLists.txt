add_executable(
    webview_docs
    WIN32
    )
target_sources(
    webview_docs
    PRIVATE "main.cxx"
            "../pane/data/app.manifest"
            "../pane/data/app.rc"
    )
target_link_libraries(
    webview_docs
    PRIVATE pane::pane
            pane::config
    )
