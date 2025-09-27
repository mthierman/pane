include(wix)
add_executable(wix)
target_sources(wix PRIVATE main.cpp ${PROJECT_SOURCE_DIR}/data/app.manifest)
target_link_libraries(console PRIVATE pane::pane)
