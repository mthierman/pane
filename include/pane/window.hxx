#pragma once
#include <Windows.h>
#include <wrl.h>
#include <chrono>
#include <functional>
#include <string>
#include <type_traits>
#include <pane/color.hxx>
#include <pane/math.hxx>
#include <pane/system.hxx>
#include <pane/text.hxx>
#include <pane/utility.hxx>
#include <wil/com.h>
#include <WebView2.h>
#include <WebView2EnvironmentOptions.h>
#include <ada.h>

namespace pane {
struct window_message final {
    using Self = window_message;

    auto send(this const Self& self, HWND hwnd = nullptr) -> LRESULT;
    auto post(this const Self& self, HWND hwnd = nullptr) -> BOOL;
    auto default_procedure(this const Self& self) -> LRESULT;

    HWND hwnd { nullptr };
    UINT msg { 0 };
    WPARAM wparam { 0 };
    LPARAM lparam { 0 };
};

template <typename M = UINT, typename W = WPARAM, typename L = LPARAM>
    requires std::is_enum_v<M> || std::is_integral_v<M>
constexpr auto make_window_message(HWND hwnd, M msg, W wparam, L lparam) {
    return window_message { hwnd, []<typename U>(U value) -> UINT {
        if constexpr (std::is_enum_v<U>) {
            return static_cast<UINT>(std::to_underlying(value));
        } else {
            return static_cast<UINT>(value);
        }
    }(msg), []<typename U>(U value) -> WPARAM {
        if constexpr (std::is_pointer_v<U>) {
            return reinterpret_cast<WPARAM>(value);
        } else {
            return static_cast<WPARAM>(value);
        }
    }(wparam), []<typename U>(U value) -> LPARAM {
        if constexpr (std::is_pointer_v<U>) {
            return reinterpret_cast<LPARAM>(value);
        } else {
            return static_cast<LPARAM>(value);
        }
    }(lparam) };
}

template <typename T> struct window_class final {
    using Self = window_class;

    window_class(std::u8string_view name)
        : name { to_utf16(name) },
          data { WNDCLASSEXW { { sizeof(WNDCLASSEXW) },
                               { 0 },
                               { class_window_procedure },
                               { 0 },
                               { sizeof(T*) },
                               { this->instance },
                               { this->icon },
                               { this->cursor },
                               { nullptr },
                               { nullptr },
                               { reinterpret_cast<const wchar_t*>(this->name.data()) },
                               { this->icon } } } {
        auto& self = *this;

        if (GetClassInfoExW(self.data.hInstance, self.data.lpszClassName, &self.data) == 0) {
            RegisterClassExW(&self.data);
        };
    }
    ~window_class() {
        auto& self = *this;

        UnregisterClassW(self.data.lpszClassName, self.data.hInstance);
    }

    window_class(const Self&) = delete;
    auto operator=(const Self&) -> Self& = delete;

    window_class(Self&&) noexcept = delete;
    auto operator=(Self&&) noexcept -> Self& = delete;

private:
    std::u16string name;
    HINSTANCE instance { system::module_handle().value_or(nullptr) };
    HICON icon { system::resource_icon().value_or(system::application_icon()) };
    HCURSOR cursor { system::arrow_cursor() };

public:
    WNDCLASSEXW data;

private:
    static auto class_window_procedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
        -> LRESULT {
        window_message window_message { hwnd, msg, wparam, lparam };

        // https://learn.microsoft.com/en-us/windows/win32/winmsg/wm-nccreate
        if (msg == WM_NCCREATE) {
            if (auto create_struct { reinterpret_cast<CREATESTRUCTW*>(window_message.lparam) }) {
                if (auto create_params { create_struct->lpCreateParams }) {
                    auto& self { *(static_cast<T*>(create_params)) };

                    SetWindowLongPtrW(window_message.hwnd, 0, reinterpret_cast<LONG_PTR>(&self));
                    self.window_handle(window_message.hwnd);
                    self.window_handle.position.dpi = GetDpiForWindow(window_message.hwnd);
                    self.window_handle.position.scale_factor
                        = static_cast<float>(self.window_handle.position.dpi)
                        / static_cast<float>(USER_DEFAULT_SCREEN_DPI);
                    SendMessageW(window_message.hwnd, WM_SETTINGCHANGE, 0, 0);
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

                    // https://learn.microsoft.com/en-us/windows/win32/hidpi/wm-dpichanged
                case WM_DPICHANGED: {
                    self.window_handle.position.dpi = HIWORD(window_message.wparam);
                    self.window_handle.position.scale_factor
                        = static_cast<float>(self.window_handle.position.dpi)
                        / static_cast<float>(USER_DEFAULT_SCREEN_DPI);
                } break;

                    // https://learn.microsoft.com/en-us/windows/win32/winmsg/wm-windowposchanged
                case WM_WINDOWPOSCHANGED: {
                    GetClientRect(window_message.hwnd, &self.window_handle.position.client_rect);

                    if (auto style { GetWindowLongPtrW(window_message.hwnd, GWL_STYLE) };
                        style & WS_OVERLAPPEDWINDOW) {
                        GetWindowPlacement(window_message.hwnd,
                                           &self.window_handle.position.window_placement);
                    }

                    WINDOWPLACEMENT window_placement { sizeof(WINDOWPLACEMENT) };
                    GetWindowPlacement(window_message.hwnd, &window_placement);

                    self.window_handle.position.maximized
                        = window_placement.showCmd == SW_SHOWMAXIMIZED;

                    self.window_handle.position.minimized
                        = window_placement.showCmd == SW_SHOWMINIMIZED;
                } break;

                    // https://learn.microsoft.com/en-us/windows/win32/winmsg/wm-settingchange
                case WM_SETTINGCHANGE: {
                    auto dark_mode { system::dark_mode() };
                    self.window_background(dark_mode ? self.window_config.bg_dark
                                                     : self.window_config.bg_light);
                    self.window_handle.immersive_dark_mode(dark_mode);
                } break;
            }

            if (self.window_procedure) {
                return self.window_procedure(window_message, self);
            }
        }

        return window_message.default_procedure();
    }
};

struct window_position final {
    using Self = window_position;

    bool fullscreen { false };
    bool maximized { false };
    bool minimized { false };
    RECT client_rect { 0, 0, 0, 0 };
    WINDOWPLACEMENT window_placement { .length { sizeof(WINDOWPLACEMENT) } };
    UINT dpi { 0 };
    float scale_factor { 0.0f };
};

enum struct window_backdrop { automatic, mica, mica_alt, acrylic, none };

struct window_background final {
    using Self = window_background;

    explicit window_background(const color& color);
    ~window_background();

    window_background(const Self&) = delete;
    auto operator=(const Self&) -> Self& = delete;

    window_background(Self&&) noexcept = delete;
    auto operator=(Self&&) noexcept -> Self& = delete;

    auto operator()(this const Self& self) -> HBRUSH;
    auto operator()(this Self& self, const color& color) -> void;

private:
    HBRUSH hbrush { nullptr };
};

struct window_handle final {
    using Self = window_handle;

    window_handle() = default;
    explicit window_handle(HWND hwnd);
    ~window_handle();

    window_handle(const Self&) = delete;
    auto operator=(const Self&) -> Self& = delete;

    window_handle(Self&&) noexcept = delete;
    auto operator=(Self&&) noexcept -> Self& = delete;

    auto activate(this const Self& self) -> bool;
    auto show(this const Self& self) -> bool;
    auto hide(this const Self& self) -> bool;

    auto title(this const Self& self, std::u8string_view title) -> bool;
    auto icon(this const Self& self, HICON icon) -> void;

    auto maximize(this const Self& self) -> bool;
    auto minimize(this const Self& self) -> bool;
    auto restore(this const Self& self) -> bool;
    auto toggle_fullscreen(this Self& self) -> bool;

    auto immersive_dark_mode(this const Self& self, bool enable) -> HRESULT;
    auto cloak(this const Self& self, bool enable) -> HRESULT;
    auto backdrop(this const Self& self, window_backdrop window_backdrop) -> HRESULT;
    auto border_color(this const Self& self, const color& color) -> HRESULT;
    auto caption_color(this const Self& self, const color& color) -> HRESULT;
    auto text_color(this const Self& self, const color& color) -> HRESULT;

    auto operator()(this const Self& self) -> HWND;
    auto operator()(this Self& self, HWND hwnd) -> void;

    window_position position;
    uintptr_t id { random_number<uintptr_t>() };
    std::chrono::steady_clock::time_point creation_time { std::chrono::steady_clock::now() };

private:
    HWND hwnd { nullptr };
};

struct window_icon final {
    using Self = window_icon;

    window_icon() = default;
    explicit window_icon(HICON hicon);
    ~window_icon();

    window_icon(const Self&) = delete;
    auto operator=(const Self&) -> Self& = delete;

    window_icon(Self&&) noexcept = delete;
    auto operator=(Self&&) noexcept -> Self& = delete;

    auto operator()(this Self& self) -> HICON&;
    auto operator()(this Self& self, HICON hicon) -> void;

private:
    HICON hicon { nullptr };
};

struct window_config final {
    std::u8string title;
    color bg_dark;
    color bg_light;
    bool visible { true };
    HWND parent_hwnd { nullptr };
};

template <typename T> using window_procedure_fn = std::function<LRESULT(const window_message&, T&)>;

struct window final {
    using Self = window;
    using procedure_fn = window_procedure_fn<Self>;

    friend struct window_class<Self>;

    window(struct window_config window_config = {}, procedure_fn window_procedure = {});
    ~window() = default;

    window(const Self&) = delete;
    auto operator=(const Self&) -> Self& = delete;

    window(Self&&) noexcept = delete;
    auto operator=(Self&&) noexcept -> Self& = delete;

    auto default_procedure(this Self& self, const window_message& window_message) -> LRESULT;

    window_config window_config;
    window_background window_background { system::dark_mode() ? window_config.bg_dark
                                                              : window_config.bg_light };
    window_handle window_handle;
    procedure_fn window_procedure;
};

// struct window_manager final {
//     using Self = window_manager;

//     auto insert(this Self& self, const window_handle& window_handle) -> void;
//     auto erase(this Self& self, const window_handle& window_handle) -> void;
//     auto clear(this Self& self) -> void;
//     auto size(this const Self& self) -> uint64_t;
//     auto contains(this const Self& self, HWND hwnd) -> bool;
//     auto empty(this const Self& self) -> bool;
//     auto first(this const Self& self) -> HWND;
//     auto last(this const Self& self) -> HWND;

// private:
//     std::set<HWND> set;
// };
} // namespace pane
