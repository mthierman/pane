add_executable(
    webview
    WIN32
    )
target_sources(webview PRIVATE "webview.cxx")
target_link_libraries(
    webview
    PRIVATE pane::pane
            pane::config
    )
