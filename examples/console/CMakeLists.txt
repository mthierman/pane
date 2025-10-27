add_executable(console)

target_sources(console PRIVATE main.cpp ${PROJECT_SOURCE_DIR}/data/app.manifest)

target_compile_features(console PRIVATE c_std_17 cxx_std_23)

set_target_properties(
    console
    PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)
