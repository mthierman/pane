add_executable(task)
target_sources(task PRIVATE "task.cpp")
target_link_libraries(task PRIVATE pane::pane)
