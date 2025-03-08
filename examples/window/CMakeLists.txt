add_executable(
    window
    WIN32
    )

target_sources(window PRIVATE "src/main.cxx")

target_link_libraries(
    window
    PRIVATE pane::pane
            pane::flags
    )
