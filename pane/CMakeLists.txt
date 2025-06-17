add_library(pane)
add_library(
    pane::pane
    ALIAS
    pane
    )
target_sources(
    pane
    PUBLIC FILE_SET
           HEADERS
           BASE_DIRS
           "src/include"
    PRIVATE "src/co_init.cxx"
            "src/color.cxx"
            "src/config.cxx"
            "src/console.cxx"
            "src/debug.cxx"
            "src/error.cxx"
            "src/file_picker.cxx"
            "src/filesystem.cxx"
            "src/gdi_plus.cxx"
            "src/glaze.cxx"
            "src/guid.cxx"
            "src/hstring.cxx"
            "src/icu.cxx"
            "src/input.cxx"
            "src/math.cxx"
            "src/message_box.cxx"
            "src/process.cxx"
            "src/string.cxx"
            "src/system.cxx"
            "src/text.cxx"
            "src/url.cxx"
            "src/window.cxx"
    )
target_link_libraries(
    pane
    PRIVATE pane::config
    PUBLIC dwmapi
           gdiplus
           Urlmon
           WindowsApp
           ada
           glaze
           webview2
           wil
           cppwinrt
    )
