#pragma once
#include <Windows.h>
#include <string>
#include <pane/text.hxx>
#include <pane/system.hxx>
#include <pane/window.hxx>

namespace pane {
template <typename T> struct message_window_class final {
    using Self = message_window_class;

    message_window_class(std::u8string_view name)
        : name { pane::to_utf16(name) },
          data { WNDCLASSEXW { { sizeof(WNDCLASSEXW) },
                               { 0 },
                               { class_window_procedure },
                               { 0 },
                               { sizeof(T*) },
                               { this->instance },
                               { nullptr },
                               { nullptr },
                               { nullptr },
                               { nullptr },
                               { reinterpret_cast<const wchar_t*>(this->name.data()) },
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

private:
    std::u16string name;
    HINSTANCE instance { pane::system::module_handle().value_or(nullptr) };

public:
    WNDCLASSEXW data;

private:
    static auto class_window_procedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
        -> LRESULT {
        pane::window_message window_message { hwnd, msg, wparam, lparam };

        // https://learn.microsoft.com/en-us/windows/win32/winmsg/wm-nccreate
        if (msg == WM_NCCREATE) {
            if (auto create_struct { reinterpret_cast<CREATESTRUCTW*>(window_message.lparam) }) {
                if (auto create_params { create_struct->lpCreateParams }) {
                    auto& self { *(static_cast<T*>(create_params)) };

                    SetWindowLongPtrW(window_message.hwnd, 0, reinterpret_cast<LONG_PTR>(&self));
                    self.window_handle(window_message.hwnd);
                }
            }
        }

        if (auto instance { GetWindowLongPtrW(hwnd, 0) }) {
            auto& self { *(reinterpret_cast<T*>(instance)) };

            switch (window_message.msg) {
                    // https://learn.microsoft.com/en-us/windows/win32/winmsg/wm-ncdestroy
                case WM_NCDESTROY: {
                    self.window_handle(nullptr);
                    SetWindowLongPtrW(window_message.hwnd, 0, reinterpret_cast<LONG_PTR>(nullptr));
                } break;
            }

            if (self.window_procedure) {
                return self.window_procedure(window_message, self);
            }
        }

        return window_message.default_procedure();
    }
};

struct message_window final {
    using Self = message_window;
    using procedure_fn = window_procedure_fn<Self>;

    friend struct message_window_class<Self>;

    message_window(procedure_fn&& window_procedure = {});
    ~message_window() = default;

    message_window(const Self&) = delete;
    auto operator=(const Self&) -> Self& = delete;

    message_window(Self&&) noexcept = delete;
    auto operator=(Self&&) noexcept -> Self& = delete;

    auto default_procedure(this Self& self, const window_message& window_message) -> LRESULT;

    window_handle window_handle;
    procedure_fn window_procedure;
};
} // namespace pane
