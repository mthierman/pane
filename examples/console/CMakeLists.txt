add_executable(console)
target_sources(console PRIVATE "main.cpp" "../../data/app.manifest")
target_link_libraries(console PRIVATE pane::pane)
