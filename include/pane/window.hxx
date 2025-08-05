#pragma once
#include <Windows.h>
#include <wrl.h>
#include <chrono>
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
          data { { sizeof(WNDCLASSEXW) },
                 { 0 },
                 { procedure },
                 { 0 },
                 { sizeof(T*) },
                 { this->instance },
                 { this->icon },
                 { this->cursor },
                 { nullptr },
                 { nullptr },
                 { reinterpret_cast<const wchar_t*>(this->name.data()) },
                 { this->icon } } {
        if (GetClassInfoExW(this->data.hInstance, this->data.lpszClassName, &this->data) == 0) {
            RegisterClassExW(&this->data);
        };
    }
    ~window_class() { UnregisterClassW(this->data.lpszClassName, this->data.hInstance); }

    window_class(const Self&) = delete;
    auto operator=(const Self&) -> Self& = delete;

    window_class(Self&&) noexcept = delete;
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
                        window->window_handle.position.dpi = GetDpiForWindow(window_message.hwnd);
                        window->window_handle.position.scale_factor
                            = static_cast<float>(window->window_handle.position.dpi)
                            / static_cast<float>(USER_DEFAULT_SCREEN_DPI);
                        window->window_handle.immersive_dark_mode(pane::system::dark_mode());
                    }
                }
            } break;

                // https://learn.microsoft.com/en-us/windows/win32/winmsg/wm-ncdestroy
            case WM_NCDESTROY: {
                SetWindowLongPtrW(window_message.hwnd, 0, reinterpret_cast<LONG_PTR>(nullptr));
            } break;
        }

        if (auto window { reinterpret_cast<T*>(GetWindowLongPtrW(hwnd, 0)) }) {
            switch (window_message.msg) {
                case WM_SETTINGCHANGE: {
                    auto dark_mode { system::dark_mode() };
                    window->window_background(dark_mode ? window->window_config.bg_dark
                                                        : window->window_config.bg_light);
                    window->window_handle.immersive_dark_mode(dark_mode);
                    RedrawWindow(
                        window_message.hwnd, nullptr, nullptr, RDW_INVALIDATE | RDW_ERASENOW);
                } break;

                    // https://learn.microsoft.com/en-us/windows/win32/hidpi/wm-dpichanged
                case WM_DPICHANGED: {
                    window->window_handle.position.dpi = HIWORD(window_message.wparam);
                    window->window_handle.position.scale_factor
                        = static_cast<float>(window->window_handle.position.dpi)
                        / static_cast<float>(USER_DEFAULT_SCREEN_DPI);

                    auto const suggested_rect { reinterpret_cast<RECT*>(window_message.lparam) };
                    SetWindowPos(window_message.hwnd,
                                 nullptr,
                                 suggested_rect->left,
                                 suggested_rect->top,
                                 suggested_rect->right - suggested_rect->left,
                                 suggested_rect->bottom - suggested_rect->top,
                                 SWP_NOZORDER | SWP_NOACTIVATE);
                } break;

                    // https://learn.microsoft.com/en-us/windows/win32/winmsg/wm-windowposchanged
                case WM_WINDOWPOSCHANGED: {
                    GetClientRect(window_message.hwnd, &window->window_handle.position.client_rect);

                    if (auto style { GetWindowLongPtrW(window_message.hwnd, GWL_STYLE) };
                        style & WS_OVERLAPPEDWINDOW) {
                        GetWindowPlacement(window_message.hwnd,
                                           &window->window_handle.position.window_placement);
                    }

                    auto window_placement { WINDOWPLACEMENT { sizeof(WINDOWPLACEMENT) } };
                    GetWindowPlacement(window_message.hwnd, &window_placement);

                    window->window_handle.position.maximized
                        = window_placement.showCmd == SW_SHOWMAXIMIZED;

                    window->window_handle.position.minimized
                        = window_placement.showCmd == SW_SHOWMINIMIZED;
                } break;

                    // https://learn.microsoft.com/en-us/windows/win32/inputdev/wm-keydown
                case WM_KEYDOWN: {
                    switch (window_message.wparam) {
                        case VK_F11: {
                            window->window_handle.toggle_fullscreen();
                        } break;
                    }
                } break;
            }

            return window->procedure(window_message);
        }

        return window_message.default_procedure();
    }

private:
    std::u16string name;
    HINSTANCE instance { system::module_handle().value_or(nullptr) };
    HICON icon { system::resource_icon().value_or(system::application_icon()) };
    HCURSOR cursor { system::arrow_cursor() };

public:
    WNDCLASSEXW data;
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

template <typename T> struct window {
    using Self = window;

    window(struct window_config window_config)
        : window_config { std::move(window_config) } {
        CreateWindowExW(
            0,
            this->window_class.data.lpszClassName,
            reinterpret_cast<const wchar_t*>(to_utf16(this->window_config.title).data()),
            this->window_config.parent_hwnd
                ? WS_CHILDWINDOW
                : WS_OVERLAPPEDWINDOW | (this->window_config.visible ? WS_VISIBLE : 0),
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            this->window_config.parent_hwnd,
            this->window_config.parent_hwnd ? reinterpret_cast<HMENU>(this->window_handle.id)
                                            : nullptr,
            this->window_class.data.hInstance,
            this);
    }
    ~window() = default;

    window(const Self&) = delete;
    auto operator=(const Self&) -> Self& = delete;

    window(Self&&) noexcept = delete;
    auto operator=(Self&&) noexcept -> Self& = delete;

    auto procedure(this T& self, const window_message& window_message) -> LRESULT {
        switch (window_message.msg) {
            case WM_ERASEBKGND: {
                RECT rect;
                GetClientRect(window_message.hwnd, &rect);
                FillRect(
                    reinterpret_cast<HDC>(window_message.wparam), &rect, self.window_background());

                return 1;
            } break;
        }

        return self.message_handler(window_message);
    }

    window_class<T> window_class { u8"pane_window" };
    window_config window_config;
    window_background window_background { pane::system::dark_mode() ? window_config.bg_dark
                                                                    : window_config.bg_light };
    window_handle window_handle;
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
