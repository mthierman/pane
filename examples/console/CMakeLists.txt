add_executable(console)

target_sources(console PRIVATE "src/main.cxx")

target_link_libraries(console PRIVATE pane::pane)
