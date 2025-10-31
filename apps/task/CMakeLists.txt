add_executable(app_task)

target_sources(app_task PRIVATE main.cpp ${PROJECT_SOURCE_DIR}/data/app.manifest)

target_link_libraries(app_task PRIVATE pane::pane)
