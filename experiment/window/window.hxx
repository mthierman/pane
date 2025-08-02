#pragma once
#include <Windows.h>
#include <expected>
#include <wil/resource.h>

namespace pane {
auto module_handle() -> std::expected<HMODULE, HRESULT>;

struct message final {
    auto default_procedure() const -> LRESULT;
    auto send(HWND hwnd) const -> LRESULT;
    auto post(HWND hwnd) const -> BOOL;

    HWND hwnd { nullptr };
    UINT msg { 0 };
    WPARAM wparam { 0 };
    LPARAM lparam { 0 };
};

template <typename T> struct window_class final {
    window_class()
        : wndclass { WNDCLASSEXW { { sizeof(WNDCLASSEXW) },
                                   { 0 },
                                   { procedure },
                                   { 0 },
                                   { sizeof(T*) },
                                   { module_handle().value_or(nullptr) },
                                   { nullptr },
                                   { nullptr },
                                   { nullptr },
                                   { nullptr },
                                   { L"windowclass" },
                                   { nullptr } } } {
        if (GetClassInfoExW(this->data().hInstance, this->data().lpszClassName, &this->data())
            == 0) {
            RegisterClassExW(&this->data());
        };
    }
    ~window_class() { UnregisterClassW(this->data().lpszClassName, this->data().hInstance); }

    window_class(const window_class&) = delete;
    auto operator=(const window_class&) -> window_class& = delete;

    window_class(window_class&&) noexcept = delete;
    auto operator=(window_class&&) noexcept -> window_class& = delete;

    auto data() const -> const WNDCLASSEXW& { return this->wndclass; }

private:
    static auto procedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> LRESULT {
        message message { hwnd, msg, wparam, lparam };

        switch (message.msg) {
                // https://learn.microsoft.com/en-us/windows/win32/winmsg/wm-nccreate
            case WM_NCCREATE: {
                if (auto create_struct { reinterpret_cast<CREATESTRUCTW*>(message.lparam) }) {
                    if (auto instance { static_cast<T*>(create_struct->lpCreateParams) }) {
                        SetWindowLongPtrW(message.hwnd, 0, reinterpret_cast<LONG_PTR>(&instance));
                        instance->hwnd.reset(message.hwnd);
                    }
                }
            } break;

                // https://learn.microsoft.com/en-us/windows/win32/winmsg/wm-ncdestroy
            case WM_NCDESTROY: {
                SetWindowLongPtrW(message.hwnd, 0, reinterpret_cast<LONG_PTR>(nullptr));
            } break;
        }

        return message.default_procedure();
    }

    WNDCLASSEXW wndclass;
};

struct message_window final {
    message_window();
    ~message_window() = default;

    message_window(const message_window&) = delete;
    auto operator=(const message_window&) -> message_window& = delete;

    message_window(message_window&&) noexcept = delete;
    auto operator=(message_window&&) noexcept -> message_window& = delete;

    auto default_procedure(const message& message) -> LRESULT;

    window_class<message_window> window_class;
    wil::unique_hwnd hwnd;
};

struct window final {
    window();
    ~window() = default;

    window(const window&) = delete;
    auto operator=(const window&) -> window& = delete;

    window(window&&) noexcept = delete;
    auto operator=(window&&) noexcept -> window& = delete;

    auto default_procedure(const message& message) -> LRESULT;

    window_class<window> window_class;
    wil::unique_hwnd hwnd;
};
} // namespace pane
