add_executable(console)

target_sources(console PRIVATE main.cpp ${PROJECT_SOURCE_DIR}/data/app.manifest)

target_link_libraries(console PRIVATE pane::pane)
