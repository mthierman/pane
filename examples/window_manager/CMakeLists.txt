add_executable(window_manager WIN32)
target_sources(
    window_manager
    PRIVATE main.cpp ${PROJECT_SOURCE_DIR}/data/app.manifest
)
target_link_libraries(window_manager PRIVATE pane::pane)
