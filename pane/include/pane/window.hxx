#pragma once
#include <Windows.h>
#include <pane/system.hxx>

namespace pane {
struct window final {
    using Self = window;

    explicit window(bool visible = true);

    static auto window_procedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> LRESULT;

    WNDCLASSEXW window_class {
        .cbSize { sizeof(WNDCLASSEXW) },
        .style { 0 },
        .lpfnWndProc { window_procedure },
        .cbClsExtra { 0 },
        .cbWndExtra { sizeof(Self) },
        .hInstance { pane::system::module_handle().value_or(nullptr) },
        .hIcon { pane::system::resource_icon().value_or(pane::system::application_icon()) },
        .hCursor { pane::system::arrow_cursor() },
        .hbrBackground { nullptr },
        .lpszMenuName { nullptr },
        .lpszClassName { L"PaneWindow" },
        .hIconSm { pane::system::resource_icon().value_or(pane::system::application_icon()) }
    };
    HWND window_handle;
};
} // namespace pane
