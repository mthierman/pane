add_executable(console)
target_sources(console PRIVATE "console.cxx")
target_link_libraries(
    console
    PRIVATE pane::pane
            pane::config
    )

add_executable(
    windows
    WIN32
    )
target_sources(windows PRIVATE "windows.cxx")
target_link_libraries(
    windows
    PRIVATE pane::pane
            pane::config
    )
