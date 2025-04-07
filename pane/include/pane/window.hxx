#pragma once
#include <Windows.h>
#include <functional>
#include <pane/system.hxx>
#include <wil/resource.h>

namespace pane {
struct window final {
    using Self = window;

    window(bool visible = true);
    explicit window(std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)>&& message_handler,
                    bool visible = true);

    static auto window_procedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> LRESULT;

    auto hwnd(this const Self& self) -> HWND;
    auto activate(this const Self& self) -> bool;

private:
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
    wil::unique_hwnd window_handle;
    std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)> message_handler {
        [](HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
        return DefWindowProcW(hwnd, msg, wparam, lparam);
    }
    };
    auto create(this Self& self, bool visible = true) -> void;
};
} // namespace pane
