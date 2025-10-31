add_executable(example_console)

target_sources(example_console PRIVATE main.cpp ${PROJECT_SOURCE_DIR}/data/app.manifest)

target_compile_features(example_console PRIVATE c_std_17 cxx_std_23)

target_link_libraries(example_console PRIVATE pane::pane)
