add_executable(task)
target_sources(task PRIVATE "task.cxx")
target_link_libraries(
    task
    PRIVATE pane::pane
            pane::config
    )
