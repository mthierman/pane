add_executable(test_strings)

target_sources(test_strings PRIVATE src/main.cpp ${PROJECT_SOURCE_DIR}/data/app.manifest)

target_compile_features(test_strings PRIVATE c_std_17 cxx_std_23)

target_link_libraries(test_strings PRIVATE pane::pane)
