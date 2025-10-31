add_executable(example_window WIN32)

target_sources(example_window PRIVATE main.cpp ${PROJECT_SOURCE_DIR}/data/app.manifest)

target_link_libraries(example_window PRIVATE pane::pane)
