add_executable(webview WIN32)
target_sources(webview PRIVATE "main.cpp" "../../data/app.manifest")
target_link_libraries(webview PRIVATE pane::pane)
target_compile_definitions(webview PRIVATE $<$<CONFIG:Debug>:DEBUG_MODE>)
