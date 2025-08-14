#pragma once
#include <Windows.h>
#include <string>
#include <pane/text.hpp>
#include <pane/system.hpp>
#include <pane/window.hpp>

namespace pane {
template <typename T> struct message_window_class final {
    using Self = message_window_class;

    message_window_class(std::u8string_view name)
        : name { to_utf16(name) },
          data { sizeof(WNDCLASSEXW),
                 0,
                 procedure,
                 0,
                 sizeof(T*),
                 this->instance,
                 nullptr,
                 nullptr,
                 nullptr,
                 nullptr,
                 reinterpret_cast<const wchar_t*>(this->name.data()),
                 nullptr } {
        if (GetClassInfoExW(this->data.hInstance, this->data.lpszClassName, &this->data) == 0) {
            RegisterClassExW(&this->data);
        };
    }
    ~message_window_class() { UnregisterClassW(this->data.lpszClassName, this->data.hInstance); }

    message_window_class(const Self&) = delete;
    auto operator=(const Self&) -> Self& = delete;

    message_window_class(Self&&) noexcept = delete;
    auto operator=(Self&&) noexcept -> Self& = delete;

private:
    static auto procedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> LRESULT {
        window_message window_message { hwnd, msg, wparam, lparam };

        switch (window_message.msg) {
                // https://learn.microsoft.com/en-us/windows/win32/winmsg/wm-nccreate
            case WM_NCCREATE: {
                if (auto create_struct {
                        reinterpret_cast<CREATESTRUCTW*>(window_message.lparam) }) {
                    if (auto window { static_cast<T*>(create_struct->lpCreateParams) }) {
                        SetWindowLongPtrW(
                            window_message.hwnd, 0, reinterpret_cast<LONG_PTR>(window));
                        window->window_handle(window_message.hwnd);
                    }
                }
            } break;

                // https://learn.microsoft.com/en-us/windows/win32/winmsg/wm-ncdestroy
            case WM_NCDESTROY: {
                SetWindowLongPtrW(window_message.hwnd, 0, reinterpret_cast<LONG_PTR>(nullptr));
            } break;
        }

        if (auto window { reinterpret_cast<T*>(GetWindowLongPtrW(hwnd, 0)) }) {
            return window->procedure(window_message);
        }

        return window_message.default_procedure();
    }

private:
    std::u16string name;
    HINSTANCE instance { pane::system::module_handle().value_or(nullptr) };

public:
    WNDCLASSEXW data;
};

template <typename T> struct message_window {
    using Self = message_window;

    message_window() {
        CreateWindowExW(0,
                        this->window_class.data.lpszClassName,
                        nullptr,
                        0,
                        0,
                        0,
                        0,
                        0,
                        HWND_MESSAGE,
                        nullptr,
                        this->window_class.data.hInstance,
                        this);
    }
    ~message_window() = default;

    message_window(const Self&) = delete;
    auto operator=(const Self&) -> Self& = delete;

    message_window(Self&&) noexcept = delete;
    auto operator=(Self&&) noexcept -> Self& = delete;

    auto procedure(this T& self, const window_message& window_message) -> LRESULT {
        return self.handle_message(window_message);
    }

    message_window_class<T> window_class { u8"pane_message_window" };
    window_handle window_handle;
};
} // namespace pane
