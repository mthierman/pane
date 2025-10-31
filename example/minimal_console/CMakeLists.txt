add_executable(example_minimal_console)

target_sources(example_minimal_console PRIVATE main.cpp ${PROJECT_SOURCE_DIR}/data/app.manifest)

target_compile_features(example_minimal_console PRIVATE c_std_17 cxx_std_23)

set_target_properties(
    example_minimal_console
    PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)
