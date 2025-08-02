#pragma once
#include <Windows.h>
#include <expected>
#include <wil/resource.h>

auto module_handle() -> std::expected<HMODULE, HRESULT>;

struct window_message final {
    using Self = window_message;

    auto default_procedure(this const Self& self) -> LRESULT;
    auto send(this const Self& self, HWND hwnd) -> LRESULT;
    auto post(this const Self& self, HWND hwnd) -> BOOL;

    HWND hwnd { nullptr };
    UINT msg { 0 };
    WPARAM wparam { 0 };
    LPARAM lparam { 0 };
};

template <typename T> struct message_window_class final {
    using Self = message_window_class;

    message_window_class()
        : data { WNDCLASSEXW { { sizeof(WNDCLASSEXW) },
                               { 0 },
                               { class_window_procedure },
                               { 0 },
                               { sizeof(T*) },
                               { module_handle().value_or(nullptr) },
                               { nullptr },
                               { nullptr },
                               { nullptr },
                               { nullptr },
                               { L"" },
                               { nullptr } } } {
        auto& self = *this;

        if (GetClassInfoExW(self.data.hInstance, self.data.lpszClassName, &self.data) == 0) {
            RegisterClassExW(&self.data);
        };
    }
    ~message_window_class() {
        auto& self = *this;

        UnregisterClassW(self.data.lpszClassName, self.data.hInstance);
    }

    message_window_class(const Self&) = delete;
    auto operator=(const Self&) -> Self& = delete;

    message_window_class(Self&&) noexcept = delete;
    auto operator=(Self&&) noexcept -> Self& = delete;

public:
    WNDCLASSEXW data;

private:
    static auto class_window_procedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
        -> LRESULT {
        window_message window_message { hwnd, msg, wparam, lparam };

        switch (window_message.msg) {
                // https://learn.microsoft.com/en-us/windows/win32/winmsg/wm-nccreate
            case WM_NCCREATE: {
                if (auto create_struct {
                        reinterpret_cast<CREATESTRUCTW*>(window_message.lparam) }) {
                    if (auto create_params { create_struct->lpCreateParams }) {
                        auto& self { *(static_cast<T*>(create_params)) };

                        SetWindowLongPtrW(
                            window_message.hwnd, 0, reinterpret_cast<LONG_PTR>(&self));
                        self.hwnd.reset(window_message.hwnd);
                    }
                }
            } break;

                // https://learn.microsoft.com/en-us/windows/win32/winmsg/wm-ncdestroy
            case WM_NCDESTROY: {
                SetWindowLongPtrW(window_message.hwnd, 0, reinterpret_cast<LONG_PTR>(nullptr));
            } break;
        }

        return window_message.default_procedure();
    }
};

struct message_window final {
    using Self = message_window;

    message_window();
    ~message_window() = default;

    message_window(const Self&) = delete;
    auto operator=(const Self&) -> Self& = delete;

    message_window(Self&&) noexcept = delete;
    auto operator=(Self&&) noexcept -> Self& = delete;

    auto default_procedure(this Self& self, const window_message& window_message) -> LRESULT;

    wil::unique_hwnd hwnd;
};

struct window final {
    using Self = window;

    window();
    ~window() = default;

    window(const Self&) = delete;
    auto operator=(const Self&) -> Self& = delete;

    window(Self&&) noexcept = delete;
    auto operator=(Self&&) noexcept -> Self& = delete;

    auto default_procedure(this Self& self, const window_message& window_message) -> LRESULT;

    wil::unique_hwnd hwnd;
};
