#pragma once
#include <Windows.h>
#include <functional>
#include <pane/system.hxx>
#include <wil/resource.h>

namespace pane {
struct window final {
    using Self = window;

    struct config final {
        std::u8string title;
        bool visible { true };
    };

    window() = default;
    ~window() = default;

    explicit window(pane::window::config&& window_config = {}, std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)>&& window_procedure = {});

    auto hwnd(this const Self& self) -> HWND;
    auto activate(this const Self& self) -> bool;

private:
    static auto class_window_procedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
        -> LRESULT;

    config window_config;
    WNDCLASSEXW window_class {
        .cbSize { sizeof(WNDCLASSEXW) },
        .style { 0 },
        .lpfnWndProc { class_window_procedure },
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
    std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)> window_procedure {
        [](HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
        return DefWindowProcW(hwnd, msg, wparam, lparam);
    }
    };
};
} // namespace pane
