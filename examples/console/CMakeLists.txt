configure_file(
    ${CMAKE_CURRENT_LIST_DIR}/generated/project.hpp.in
    ${CMAKE_CURRENT_BINARY_DIR}/generated/project.hpp
    @ONLY
)

add_executable(console)

target_include_directories(console PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/generated)

target_sources(console PRIVATE main.cpp ${PROJECT_SOURCE_DIR}/data/app.manifest)

target_link_libraries(console PRIVATE pane::pane)
