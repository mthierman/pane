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
           "include"
    PRIVATE "internal/co_init.cpp"
            "internal/color.cpp"
            "internal/config.cpp"
            "internal/console.cpp"
            "internal/debug.cpp"
            "internal/error.cpp"
            "internal/file_picker.cpp"
            "internal/filesystem.cpp"
            "internal/gdi_plus.cpp"
            "internal/guid.cpp"
            "internal/hstring.cpp"
            "internal/icu.cpp"
            "internal/input.cpp"
            "internal/math.cpp"
            "internal/message_box.cpp"
            "internal/process.cpp"
            "internal/string.cpp"
            "internal/system.cpp"
            "internal/text.cpp"
            "internal/url.cpp"
            "internal/webview.cpp"
            "internal/window.cpp"
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
