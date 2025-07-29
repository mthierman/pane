#include <pane/input.hxx>
#include <pane/window.hxx>
#include <pane/text.hxx>
#include <dwmapi.h>
#include <wil/wrl.h>

#include <pane/debug.hxx>

namespace pane {
auto window_message::default_procedure(this const Self& self) -> LRESULT {
    return DefWindowProcW(self.hwnd, self.event, self.wparam, self.lparam);
}

window_handle::~window_handle() { DestroyWindow(this->hwnd); }

auto window_handle::activate(this const Self& self) -> bool {
    return ShowWindow(self.hwnd, SW_NORMAL);
}

auto window_handle::show(this const Self& self) -> bool { return ShowWindow(self.hwnd, SW_SHOW); }

auto window_handle::hide(this const Self& self) -> bool { return ShowWindow(self.hwnd, SW_HIDE); }

auto window_handle::maximize(this const Self& self) -> bool {
    return ShowWindow(self.hwnd, SW_MAXIMIZE);
}

auto window_handle::minimize(this const Self& self) -> bool {
    return ShowWindow(self.hwnd, SW_MINIMIZE);
}

auto window_handle::restore(this const Self& self) -> bool {
    return ShowWindow(self.hwnd, SW_RESTORE);
}

// https://devblogs.microsoft.com/oldnewthing/20100412-00/?p=14353
auto window_handle::toggle_fullscreen(this Self& self) -> bool {
    auto style { GetWindowLongPtrW(self.hwnd, GWL_STYLE) };

    if (style & WS_OVERLAPPEDWINDOW) {
        MONITORINFO monitor_info { .cbSize { sizeof(MONITORINFO) } };

        if (GetWindowPlacement(self.hwnd, &self.window_position.window_placement)
            && GetMonitorInfoW(MonitorFromWindow(self.hwnd, MONITOR_DEFAULTTONEAREST),
                               &monitor_info)) {
            SetWindowLongPtrW(self.hwnd, GWL_STYLE, style & ~WS_OVERLAPPEDWINDOW);
            auto& monitor { monitor_info.rcMonitor };
            SetWindowPos(self.hwnd,
                         HWND_TOP,
                         monitor.left,
                         monitor.top,
                         (monitor.right - monitor.left),
                         (monitor.bottom - monitor.top),
                         SWP_NOOWNERZORDER | SWP_FRAMECHANGED);

            self.window_position.fullscreen = true;

            return true;
        }
    }

    if (!(style & WS_OVERLAPPEDWINDOW)) {
        SetWindowLongPtrW(self.hwnd, GWL_STYLE, style | WS_OVERLAPPEDWINDOW);
        SetWindowPlacement(self.hwnd, &self.window_position.window_placement);
        auto& restore { self.window_position.window_placement.rcNormalPosition };
        SetWindowPos(self.hwnd,
                     HWND_TOP,
                     restore.left,
                     restore.top,
                     (restore.right - restore.left),
                     (restore.bottom - restore.top),
                     SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
    }

    self.window_position.fullscreen = false;

    return false;
}

auto window_handle::immersive_dark_mode(this const Self& self, bool dark_mode) -> HRESULT {
    BOOL attribute { dark_mode };

    return DwmSetWindowAttribute(self.hwnd,
                                 DWMWINDOWATTRIBUTE::DWMWA_USE_IMMERSIVE_DARK_MODE,
                                 &attribute,
                                 sizeof(attribute));
}

auto window_handle::cloak(this const Self& self, bool cloak) -> HRESULT {
    BOOL attribute { cloak };

    return DwmSetWindowAttribute(
        self.hwnd, DWMWINDOWATTRIBUTE::DWMWA_CLOAK, &attribute, sizeof(attribute));
}

auto window_handle::backdrop(this const Self& self, pane::window_backdrop window_backdrop)
    -> HRESULT {
    auto backdrop { DWM_SYSTEMBACKDROP_TYPE::DWMSBT_AUTO };

    MARGINS margins { -1, -1, -1, -1 };
    DwmExtendFrameIntoClientArea(self.hwnd, &margins);

    switch (window_backdrop) {
        case pane::window_backdrop::automatic: {
            backdrop = DWM_SYSTEMBACKDROP_TYPE::DWMSBT_AUTO;
            MARGINS margins { 0, 0, 0, 0 };
            DwmExtendFrameIntoClientArea(self.hwnd, &margins);
        } break;
        case pane::window_backdrop::mica: {
            backdrop = DWM_SYSTEMBACKDROP_TYPE::DWMSBT_MAINWINDOW;
            MARGINS margins { -1, -1, -1, -1 };
            DwmExtendFrameIntoClientArea(self.hwnd, &margins);
        } break;
        case pane::window_backdrop::mica_alt: {
            backdrop = DWM_SYSTEMBACKDROP_TYPE::DWMSBT_TABBEDWINDOW;
            MARGINS margins { -1, -1, -1, -1 };
            DwmExtendFrameIntoClientArea(self.hwnd, &margins);
        } break;
        case pane::window_backdrop::acrylic: {
            backdrop = DWM_SYSTEMBACKDROP_TYPE::DWMSBT_TRANSIENTWINDOW;
            MARGINS margins { -1, -1, -1, -1 };
            DwmExtendFrameIntoClientArea(self.hwnd, &margins);
        } break;
        case pane::window_backdrop::none: {
            backdrop = DWM_SYSTEMBACKDROP_TYPE::DWMSBT_NONE;
            MARGINS margins { 0, 0, 0, 0 };
            DwmExtendFrameIntoClientArea(self.hwnd, &margins);
        } break;
    }

    return DwmSetWindowAttribute(self.hwnd, backdrop, &backdrop, sizeof(backdrop));
}

auto window_handle::border_color(this const Self& self, const pane::color& color) -> HRESULT {
    auto caption_color { color.to_colorref() };

    return DwmSetWindowAttribute(
        self.hwnd, DWMWINDOWATTRIBUTE::DWMWA_BORDER_COLOR, &caption_color, sizeof(caption_color));
}

auto window_handle::caption_color(this const Self& self, const pane::color& color) -> HRESULT {
    auto caption_color { color.to_colorref() };

    return DwmSetWindowAttribute(
        self.hwnd, DWMWINDOWATTRIBUTE::DWMWA_CAPTION_COLOR, &caption_color, sizeof(caption_color));
}

auto window_handle::text_color(this const Self& self, const pane::color& color) -> HRESULT {
    auto text_color { color.to_colorref() };

    return DwmSetWindowAttribute(
        self.hwnd, DWMWINDOWATTRIBUTE::DWMWA_TEXT_COLOR, &text_color, sizeof(text_color));
}

auto window_handle::operator()(this const Self& self) -> HWND { return self.hwnd; }

auto window_handle::operator()(this Self& self, HWND hwnd) -> void {
    if (!self.hwnd) {
        self.hwnd = hwnd;
    }
}

window_background::window_background(const pane::color& color)
    : hbrush { color.to_hbrush() } { }

window_background::~window_background() { DeleteObject(this->hbrush); }

auto window_background::operator()(this const Self& self) -> HBRUSH { return self.hbrush; }

auto window_background::operator()(this Self& self, const pane::color& color) -> void {
    if (!self.hbrush) {
        self.hbrush = color.to_hbrush();
    } else {
        DeleteObject(self.hbrush);
        self.hbrush = color.to_hbrush();
    }
}

window_icon::window_icon(HICON hicon)
    : hicon { hicon } { }

window_icon::~window_icon() { DestroyIcon(this->hicon); }

auto window_icon::operator()(this Self& self) -> HICON& { return self.hicon; }

auto window_icon::operator()(this Self& self, HICON hicon) -> void {
    if (!self.hicon) {
        self.hicon = hicon;
    } else {
        DestroyIcon(self.hicon);
        self.hicon = hicon;
    }
}

window::window(pane::window_config&& window_config,
               std::function<LRESULT(Self*, pane::window_message)>&& window_procedure)
    : window_procedure { std::move(window_procedure) },
      window_config { std::move(window_config) } {
    auto& self = *this;

    CreateWindowExW(
        0,
        self.window_class().lpszClassName,
        reinterpret_cast<const wchar_t*>(pane::to_utf16(self.window_config.title).data()),
        self.window_config.parent_hwnd
            ? WS_CHILDWINDOW
            : WS_OVERLAPPEDWINDOW | (self.window_config.visible ? WS_VISIBLE : 0),
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        self.window_config.parent_hwnd,
        self.window_config.parent_hwnd ? reinterpret_cast<HMENU>(self.id) : nullptr,
        self.window_class().hInstance,
        &self);
}

auto window::default_procedure(this Self& self, const pane::window_message& window_message)
    -> LRESULT {
    switch (window_message.event) {
            // https://learn.microsoft.com/en-us/windows/win32/hidpi/wm-dpichanged
        case WM_DPICHANGED: {
            self.dpi = HIWORD(window_message.wparam);
            self.scale_factor
                = static_cast<float>(self.dpi) / static_cast<float>(USER_DEFAULT_SCREEN_DPI);

            auto const suggested_rect { reinterpret_cast<RECT*>(window_message.lparam) };
            SetWindowPos(self.window_handle(),
                         nullptr,
                         suggested_rect->left,
                         suggested_rect->top,
                         suggested_rect->right - suggested_rect->left,
                         suggested_rect->bottom - suggested_rect->top,
                         SWP_NOZORDER | SWP_NOACTIVATE);

            return 0;
        } break;

            // https://learn.microsoft.com/en-us/windows/win32/winmsg/wm-erasebkgnd
        case WM_ERASEBKGND: {
            GetClientRect(self.window_handle(), &self.window_handle.window_position.client_rect);

            FillRect(reinterpret_cast<HDC>(window_message.wparam),
                     &self.window_handle.window_position.client_rect,
                     self.window_background());

            return 1;
        } break;

            // https://learn.microsoft.com/en-us/windows/win32/inputdev/wm-keydown
        case WM_KEYDOWN: {
            switch (window_message.wparam) {
                case VK_F11: {
                    self.window_handle.toggle_fullscreen();

                    return 0;
                } break;
            }
        } break;

            // https://learn.microsoft.com/en-us/windows/win32/winmsg/wm-settingchange
        case WM_SETTINGCHANGE: {
            self.set_theme();

            return 0;
        } break;

            // https://learn.microsoft.com/en-us/windows/win32/menurc/wm-syscommand
        case WM_SYSCOMMAND: {
            if (self.window_handle.window_position.fullscreen) {
                return 0;
            }
        } break;

            // https://learn.microsoft.com/en-us/windows/win32/winmsg/wm-windowposchanged
        case WM_WINDOWPOSCHANGED: {
            GetClientRect(self.window_handle(), &self.window_handle.window_position.client_rect);

            if (auto style { GetWindowLongPtrW(self.window_handle(), GWL_STYLE) };
                style & WS_OVERLAPPEDWINDOW) {
                GetWindowPlacement(self.window_handle(),
                                   &self.window_handle.window_position.window_placement);
            }

            WINDOWPLACEMENT window_placement { .length { sizeof(WINDOWPLACEMENT) } };
            GetWindowPlacement(self.window_handle(), &window_placement);

            if (window_placement.showCmd == SW_SHOWMAXIMIZED) {
                self.window_handle.window_position.maximized = true;
            } else {
                self.window_handle.window_position.maximized = false;
            }

            if (window_placement.showCmd == SW_SHOWMINIMIZED) {
                self.window_handle.window_position.minimized = true;
            } else {
                self.window_handle.window_position.minimized = false;
            }

            return 0;
        } break;

        default: {
            return DefWindowProcW(self.window_handle(),
                                  window_message.event,
                                  window_message.wparam,
                                  window_message.lparam);
        }
    }

    return DefWindowProcW(
        self.window_handle(), window_message.event, window_message.wparam, window_message.lparam);
}

auto window::window_class_procedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> LRESULT {
    // https://learn.microsoft.com/en-us/windows/win32/winmsg/wm-nccreate
    if (msg == WM_NCCREATE) {
        if (auto create_struct { reinterpret_cast<CREATESTRUCTW*>(lparam) }) {
            if (auto self { static_cast<Self*>(create_struct->lpCreateParams) }) {
                SetWindowLongPtrW(hwnd, 0, reinterpret_cast<LONG_PTR>(self));
                self->window_handle(hwnd);
                self->set_theme();
            }
        }
    }

    // https://learn.microsoft.com/en-us/windows/win32/winmsg/wm-ncdestroy
    if (msg == WM_NCDESTROY) {
        if (auto self { reinterpret_cast<Self*>(GetWindowLongPtrW(hwnd, 0)) }) {
            self->window_handle(nullptr);
            SetWindowLongPtrW(hwnd, 0, reinterpret_cast<LONG_PTR>(nullptr));
        }
    }

    if (auto self { reinterpret_cast<Self*>(GetWindowLongPtrW(hwnd, 0)) }) {
        if (self->window_procedure) {
            return self->window_procedure(self, { hwnd, msg, wparam, lparam });
        }
    }

    return DefWindowProcW(hwnd, msg, wparam, lparam);
}

auto window::set_theme(this Self& self) -> void {
    auto dark_mode { pane::system::dark_mode() };

    self.window_background(dark_mode ? self.window_config.dark_background
                                     : self.window_config.light_background);

    self.window_handle.immersive_dark_mode(dark_mode);

    InvalidateRect(self.window_handle(), nullptr, true);
}

auto window_manager::insert(this Self& self, const pane::window_handle& window_handle) -> void {
    self.set.insert(window_handle());
}

auto window_manager::erase(this Self& self, const pane::window_handle& window_handle) -> void {
    self.set.erase(window_handle());

    if (self.set.empty()) {
        PostQuitMessage(0);
    }
}

auto window_manager::clear(this Self& self) -> void {
    self.set.clear();

    if (self.set.empty()) {
        PostQuitMessage(0);
    }
}

auto window_manager::size(this const Self& self) -> uint64_t { return self.set.size(); }

auto window_manager::contains(this const Self& self, HWND hwnd) -> bool {
    return self.set.contains(hwnd);
}

auto window_manager::empty(this const Self& self) -> bool { return self.set.empty(); }

auto window_manager::first(this const Self& self) -> HWND { return *self.set.begin(); }

auto window_manager::last(this const Self& self) -> HWND { return *self.set.end(); }
} // namespace pane
