add_executable(example_webview_navigation WIN32)

target_sources(example_webview_navigation PRIVATE main.cpp ${PROJECT_SOURCE_DIR}/data/app.manifest)

target_link_libraries(example_webview_navigation PRIVATE pane::pane pane::config)
