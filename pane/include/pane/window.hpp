#pragma once
#include <Windows.h>
#include <wrl.h>
#include <chrono>
#include <string>
#include <type_traits>
#include <vector>
#include <pane/color.hpp>
#include <pane/input.hpp>
#include <pane/math.hpp>
#include <pane/system.hpp>
#include <pane/text.hpp>
#include <pane/utility.hpp>
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

struct window_handle final {
    using Self = window_handle;

    window_handle() = default;
    explicit window_handle(HWND hwnd);
    ~window_handle();

    window_handle(const Self&) = delete;
    auto operator=(const Self&) -> Self& = delete;

    window_handle(Self&&) noexcept = delete;
    auto operator=(Self&&) noexcept -> Self& = delete;

    auto activate(this const Self& self, bool visible) -> bool;
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

template <typename T> struct window_manager final {
    using Self = window_manager;

    window_manager() { this->create(); }
    ~window_manager() = default;

    window_manager(const Self&) = delete;
    auto operator=(const Self&) -> Self& = delete;

    window_manager(Self&&) noexcept = delete;
    auto operator=(Self&&) noexcept -> Self& = delete;

    auto create(this Self& self) -> void { self.windows.push_back(std::make_unique<T>(&self)); }

    template <typename U> auto destroy(this Self& self, U& window) -> void {
        std::erase_if(self.windows, [&](const auto& w) {
            return w && w->window_handle() == window.window_handle();
        });

        if (self.windows.empty()) {
            pane::system::quit();
        }
    }

    auto clear(this Self& self) -> void {
        self.windows.clear();

        if (self.windows.empty()) {
            pane::system::quit();
        }
    }

    auto size(this const Self& self) -> uint64_t { return self.windows.size(); }

    auto empty(this const Self& self) -> bool { return self.windows.empty(); }

    auto first(this const Self& self) -> T* { return self.windows.front(); }

    auto last(this const Self& self) -> T* { return self.windows.back(); }

private:
    std::vector<std::unique_ptr<T>> windows;
};

template <typename T> struct window_class final {
    using Self = window_class;

    window_class(std::u8string_view name)
        : name { to_utf16(name) },
          data { sizeof(WNDCLASSEXW),
                 0,
                 procedure,
                 0,
                 sizeof(T*),
                 this->instance,
                 this->icon,
                 this->cursor,
                 nullptr,
                 nullptr,
                 reinterpret_cast<const wchar_t*>(this->name.data()),
                 this->icon } {
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
                        SendMessageW(window_message.hwnd, WM_SETTINGCHANGE, 0, 0);
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
                    // https://learn.microsoft.com/en-us/windows/win32/winmsg/wm-settingchange
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
                    BOOL was_key_down = (HIWORD(window_message.lparam) & KF_REPEAT) == KF_REPEAT;

                    switch (window_message.wparam) {
                        case VK_F11: {
                            if (!(was_key_down)) {
                                window->window_handle.toggle_fullscreen();
                            }
                        } break;
                        case 'N': {
                            if (pane::input::is_key_down(VK_LCONTROL)
                                || pane::input::is_key_down(VK_RCONTROL)) {
                                if (window->window_manager) {
                                    if (!(was_key_down)) {
                                        window->window_manager->create();
                                    }
                                }
                            }
                        } break;
                        case 'W': {
                            if (pane::input::is_key_down(VK_LCONTROL)
                                || pane::input::is_key_down(VK_RCONTROL)) {
                                if (!(was_key_down)) {
                                    SendMessageW(window_message.hwnd, WM_CLOSE, 0, 0);
                                }
                            }
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

struct window_config final {
    std::u8string title;
    color bg_dark;
    color bg_light;
    bool visible { true };
    HWND parent_hwnd { nullptr };
};

template <typename T> struct window {
    using Self = window;

    struct config {
        window_config window;
    };

    window(struct window_manager<T>* window_manager = nullptr)
        : window_manager { window_manager } {
        this->window_config = T::config().window;

        CreateWindowExW(
            0,
            this->window_class.data.lpszClassName,
            reinterpret_cast<const wchar_t*>(to_utf16(this->window_config.title).data()),
            this->window_config.parent_hwnd ? WS_CHILDWINDOW | WS_CLIPSIBLINGS
                                            : WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            this->window_config.parent_hwnd,
            this->window_config.parent_hwnd ? reinterpret_cast<HMENU>(this->window_handle.id)
                                            : nullptr,
            this->window_class.data.hInstance,
            this);

        this->window_handle.activate(this->window_config.visible);
    }
    ~window() = default;

    window(const Self&) = delete;
    auto operator=(const Self&) -> Self& = delete;

    window(Self&&) noexcept = delete;
    auto operator=(Self&&) noexcept -> Self& = delete;

    auto procedure(this T& self, const window_message& window_message) -> LRESULT {
        return self.handle_message(window_message);
    }

    auto default_procedure(this T& self, const window_message& window_message) -> LRESULT {
        switch (window_message.msg) {
                // https://learn.microsoft.com/en-us/windows/win32/winmsg/wm-erasebkgnd
            case WM_ERASEBKGND: {
                RECT client_rect;
                GetClientRect(window_message.hwnd, &client_rect);
                FillRect(reinterpret_cast<HDC>(window_message.wparam),
                         &client_rect,
                         self.window_background());

                return 1;
            } break;

                // https://learn.microsoft.com/en-us/windows/win32/winmsg/wm-close
            case WM_CLOSE: {
                if (self.window_manager) {
                    self.window_manager->destroy(self);
                } else {
                    DestroyWindow(window_message.hwnd);
                }

                return 0;
            } break;

                // https://learn.microsoft.com/en-us/windows/win32/winmsg/wm-destroy
            case WM_DESTROY: {
                if (!self.window_manager) {
                    system::quit();
                }

                return 0;
            } break;
        }

        return window_message.default_procedure();
    }

    window_class<T> window_class { u8"pane_window" };
    window_config window_config;
    window_background window_background { pane::system::dark_mode() ? window_config.bg_dark
                                                                    : window_config.bg_light };
    window_handle window_handle;
    window_manager<T>* window_manager { nullptr };
};
} // namespace pane
