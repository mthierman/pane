add_executable(json)

target_sources(json PRIVATE "src/main.cxx")

target_link_libraries(
    json
    PRIVATE pane::pane
            pane::flags
            glaze::glaze
    )
