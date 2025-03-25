#pragma once
#include <Windows.h>
#include <pane/system.hxx>

namespace pane {
struct window {
    using Self = window;

    window(bool visible = true);

    static auto CALLBACK window_procedure(::HWND hwnd, ::UINT msg, ::WPARAM wparam, ::LPARAM lparam)
        -> ::LRESULT;

    ::WNDCLASSEXW window_class { .cbSize { sizeof(::WNDCLASSEXW) },
                                 .style { 0 },
                                 .lpfnWndProc { window_procedure },
                                 .cbClsExtra { 0 },
                                 .cbWndExtra { sizeof(Self) },
                                 .hInstance {
                                     pane::module_handle().value_or(GetModuleHandleW(nullptr)) },
                                 .hIcon { pane::application_icon() },
                                 .hCursor { pane::arrow_cursor() },
                                 .hbrBackground { nullptr },
                                 .lpszMenuName { nullptr },
                                 .lpszClassName { L"DefaultWindow" },
                                 .hIconSm { pane::application_icon() } };
    ::HWND window_handle;
};
} // namespace pane
