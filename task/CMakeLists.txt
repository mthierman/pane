add_executable(task)

target_sources(task PRIVATE main.cpp "${PROJECT_SOURCE_DIR}/data/app.manifest")

target_link_libraries(task PRIVATE pane::pane)
