add_executable(filesystem_test)

target_sources(filesystem_test PRIVATE src/main.cpp ${PROJECT_SOURCE_DIR}/data/app.manifest)

target_compile_features(filesystem_test PRIVATE c_std_17 cxx_std_23)

target_link_libraries(filesystem_test PRIVATE pane::pane)
