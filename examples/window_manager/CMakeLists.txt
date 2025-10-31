add_executable(example_window_manager WIN32)

target_sources(example_window_manager PRIVATE main.cpp ${PROJECT_SOURCE_DIR}/data/app.manifest)

target_link_libraries(example_window_manager PRIVATE pane::pane)
