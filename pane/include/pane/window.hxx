#pragma once
#include <Windows.h>
#include <functional>
#include <pane/system.hxx>

namespace pane {
struct window final {
    using Self = window;

    explicit window();
    explicit window(std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)>&& message_handler);

    static auto window_procedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> LRESULT;

    auto register_class(this Self& self) -> void;
    auto create(this Self& self) -> HWND;
    auto activate(this Self& self) -> bool;

    HWND window_handle { nullptr };
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
    std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)> message_handler {
        [](HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
        return DefWindowProcW(hwnd, msg, wparam, lparam);
    }
    };
};
} // namespace pane
