add_executable(test_filesystem)

target_sources(test_filesystem PRIVATE src/main.cpp ${PROJECT_SOURCE_DIR}/data/app.manifest)

target_compile_features(test_filesystem PRIVATE c_std_17 cxx_std_23)

target_link_libraries(test_filesystem PRIVATE pane::pane)
