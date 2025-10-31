add_executable(browser WIN32)

target_sources(browser PRIVATE main.cpp ${PROJECT_SOURCE_DIR}/data/app.manifest)

target_link_libraries(browser PRIVATE pane::pane)

install(TARGETS browser RUNTIME DESTINATION .)
