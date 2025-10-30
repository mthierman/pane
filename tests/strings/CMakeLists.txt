add_executable(strings_test)

target_sources(strings_test PRIVATE src/main.cpp ${PROJECT_SOURCE_DIR}/data/app.manifest)

target_compile_features(strings_test PRIVATE c_std_17 cxx_std_23)

target_link_libraries(strings_test PRIVATE pane::pane)
