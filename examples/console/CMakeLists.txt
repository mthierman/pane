add_executable(console)

target_sources(console PRIVATE main.cpp ${PROJECT_SOURCE_DIR}/data/app.manifest)

target_compile_features(console PRIVATE c_std_17 cxx_std_23)

set_target_properties(
    console
    PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)

# target_link_libraries(console PRIVATE pane::pane)
# target_link_libraries(console PRIVATE icu)
# target_link_libraries(
#     pane
#     PUBLIC
#         dwmapi
#         gdiplus
#         Urlmon
#         WindowsApp
#         ada::ada-url
#         microsoft::cppwinrt
#         microsoft::webview2
#         microsoft::wil
#         sqlite::sqlite
#         stephenberry::argz
#         stephenberry::glaze
# )
