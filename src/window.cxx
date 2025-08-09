#include <pane/window.hxx>
#include <dwmapi.h>

namespace pane {
auto window_message::send(this const Self& self, HWND hwnd) -> LRESULT {
    return SendMessageW(hwnd ? hwnd : self.hwnd, self.msg, self.wparam, self.lparam);
}

auto window_message::post(this const Self& self, HWND hwnd) -> BOOL {
    return PostMessageW(hwnd ? hwnd : self.hwnd, self.msg, self.wparam, self.lparam);
}

auto window_message::default_procedure(this const Self& self) -> LRESULT {
    return DefWindowProcW(self.hwnd, self.msg, self.wparam, self.lparam);
}

window_background::window_background(const color& color)
    : hbrush { color.to_hbrush() } { }

window_background::~window_background() { DeleteObject(this->hbrush); }

auto window_background::operator()(this const Self& self) -> HBRUSH { return self.hbrush; }

auto window_background::operator()(this Self& self, const color& color) -> void {
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

window_handle::window_handle(HWND hwnd)
    : hwnd { hwnd } { }

window_handle::~window_handle() { DestroyWindow(this->hwnd); }

auto window_handle::activate(this const Self& self, bool visible) -> bool {
    bool activated { false };

    self.cloak(true);

    if (visible) {
        activated = ShowWindow(self.hwnd, SW_NORMAL);
    } else {
        activated = ShowWindow(self.hwnd, SW_NORMAL);
        ShowWindow(self.hwnd, SW_HIDE);
    }

    self.cloak(false);

    return activated;
}

auto window_handle::show(this const Self& self) -> bool { return ShowWindow(self.hwnd, SW_SHOW); }

auto window_handle::hide(this const Self& self) -> bool { return ShowWindow(self.hwnd, SW_HIDE); }

auto window_handle::title(this const Self& self, std::u8string_view title) -> bool {
    return SetWindowTextW(self.hwnd, reinterpret_cast<const wchar_t*>(to_utf16(title).data()));
}

auto window_handle::icon(this const Self& self, HICON icon) -> void {
    SendMessageW(self.hwnd, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(icon));
    SendMessageW(self.hwnd, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(icon));
}

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

        if (GetWindowPlacement(self.hwnd, &self.position.window_placement)
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

            self.position.fullscreen = true;

            return true;
        }
    }

    if (!(style & WS_OVERLAPPEDWINDOW)) {
        SetWindowLongPtrW(self.hwnd, GWL_STYLE, style | WS_OVERLAPPEDWINDOW);
        SetWindowPlacement(self.hwnd, &self.position.window_placement);
        auto& restore { self.position.window_placement.rcNormalPosition };
        SetWindowPos(self.hwnd,
                     HWND_TOP,
                     restore.left,
                     restore.top,
                     (restore.right - restore.left),
                     (restore.bottom - restore.top),
                     SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
    }

    self.position.fullscreen = false;

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

auto window_handle::backdrop(this const Self& self, window_backdrop window_backdrop) -> HRESULT {
    auto backdrop { DWM_SYSTEMBACKDROP_TYPE::DWMSBT_AUTO };

    MARGINS margins { -1, -1, -1, -1 };
    DwmExtendFrameIntoClientArea(self.hwnd, &margins);

    switch (window_backdrop) {
        case window_backdrop::automatic: {
            backdrop = DWM_SYSTEMBACKDROP_TYPE::DWMSBT_AUTO;
            MARGINS margins { 0, 0, 0, 0 };
            DwmExtendFrameIntoClientArea(self.hwnd, &margins);
        } break;
        case window_backdrop::mica: {
            backdrop = DWM_SYSTEMBACKDROP_TYPE::DWMSBT_MAINWINDOW;
            MARGINS margins { -1, -1, -1, -1 };
            DwmExtendFrameIntoClientArea(self.hwnd, &margins);
        } break;
        case window_backdrop::mica_alt: {
            backdrop = DWM_SYSTEMBACKDROP_TYPE::DWMSBT_TABBEDWINDOW;
            MARGINS margins { -1, -1, -1, -1 };
            DwmExtendFrameIntoClientArea(self.hwnd, &margins);
        } break;
        case window_backdrop::acrylic: {
            backdrop = DWM_SYSTEMBACKDROP_TYPE::DWMSBT_TRANSIENTWINDOW;
            MARGINS margins { -1, -1, -1, -1 };
            DwmExtendFrameIntoClientArea(self.hwnd, &margins);
        } break;
        case window_backdrop::none: {
            backdrop = DWM_SYSTEMBACKDROP_TYPE::DWMSBT_NONE;
            MARGINS margins { 0, 0, 0, 0 };
            DwmExtendFrameIntoClientArea(self.hwnd, &margins);
        } break;
    }

    return DwmSetWindowAttribute(self.hwnd, backdrop, &backdrop, sizeof(backdrop));
}

auto window_handle::border_color(this const Self& self, const color& color) -> HRESULT {
    auto caption_color { color.to_colorref() };

    return DwmSetWindowAttribute(
        self.hwnd, DWMWINDOWATTRIBUTE::DWMWA_BORDER_COLOR, &caption_color, sizeof(caption_color));
}

auto window_handle::caption_color(this const Self& self, const color& color) -> HRESULT {
    auto caption_color { color.to_colorref() };

    return DwmSetWindowAttribute(
        self.hwnd, DWMWINDOWATTRIBUTE::DWMWA_CAPTION_COLOR, &caption_color, sizeof(caption_color));
}

auto window_handle::text_color(this const Self& self, const color& color) -> HRESULT {
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
} // namespace pane
