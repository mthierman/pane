add_executable(${PROJECT_NAME}_example_console)
target_sources(${PROJECT_NAME}_example_console PRIVATE "src/console.cxx")
target_link_libraries(
    ${PROJECT_NAME}_example_console
    PRIVATE ${PROJECT_NAME}::${PROJECT_NAME}
            ${PROJECT_NAME}::config
    )

add_executable(${PROJECT_NAME}_example_co_init)
target_sources(${PROJECT_NAME}_example_co_init PRIVATE "src/co_init.cxx")
target_link_libraries(
    ${PROJECT_NAME}_example_co_init
    PRIVATE ${PROJECT_NAME}::${PROJECT_NAME}
            ${PROJECT_NAME}::config
    )

add_executable(${PROJECT_NAME}_example_library)
target_sources(${PROJECT_NAME}_example_library PRIVATE "src/library.cxx")
target_link_libraries(
    ${PROJECT_NAME}_example_library
    PRIVATE ${PROJECT_NAME}::${PROJECT_NAME}
            ${PROJECT_NAME}::config
    )

add_executable(${PROJECT_NAME}_example_file)
target_sources(${PROJECT_NAME}_example_file PRIVATE "src/file.cxx")
target_link_libraries(
    ${PROJECT_NAME}_example_file
    PRIVATE ${PROJECT_NAME}::${PROJECT_NAME}
            ${PROJECT_NAME}::config
    )

add_executable(${PROJECT_NAME}_example_url)
target_sources(${PROJECT_NAME}_example_url PRIVATE "src/url.cxx")
target_link_libraries(
    ${PROJECT_NAME}_example_url
    PRIVATE ${PROJECT_NAME}::${PROJECT_NAME}
            ${PROJECT_NAME}::config
    )

add_executable(${PROJECT_NAME}_example_color)
target_sources(${PROJECT_NAME}_example_color PRIVATE "src/color.cxx")
target_link_libraries(
    ${PROJECT_NAME}_example_color
    PRIVATE ${PROJECT_NAME}::${PROJECT_NAME}
            ${PROJECT_NAME}::config
    )

add_executable(${PROJECT_NAME}_example_json)
target_sources(${PROJECT_NAME}_example_json PRIVATE "src/json.cxx")
target_link_libraries(
    ${PROJECT_NAME}_example_json
    PRIVATE ${PROJECT_NAME}::${PROJECT_NAME}
            ${PROJECT_NAME}::config
            ${PROJECT_NAME}::glaze
    )

add_executable(
    ${PROJECT_NAME}_example_window
    WIN32
    )
target_sources(${PROJECT_NAME}_example_window PRIVATE "src/window.cxx")
target_link_libraries(
    ${PROJECT_NAME}_example_window
    PRIVATE ${PROJECT_NAME}::${PROJECT_NAME}
            ${PROJECT_NAME}::config
    )

add_executable(
    ${PROJECT_NAME}_example_file_picker
    WIN32
    )
target_sources(${PROJECT_NAME}_example_file_picker PRIVATE "src/file_picker.cxx")
target_link_libraries(
    ${PROJECT_NAME}_example_file_picker
    PRIVATE ${PROJECT_NAME}::${PROJECT_NAME}
            ${PROJECT_NAME}::config
    )
