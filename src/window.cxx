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
    this->create();
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

auto window::create(this Self& self) -> HWND {
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

    return self.window_handle();
}

auto webview_token::operator()(this Self& self) -> EventRegistrationToken* { return &self.token; }

webview::webview(pane::window_config&& window_config,
                 pane::webview_config&& webview_config,
                 std::function<LRESULT(Self*, pane::window_message)>&& window_procedure)
    : window_procedure { std::move(window_procedure) },
      window_config { std::move(window_config) },
      webview_config { std::move(webview_config) } {
    this->create();
}

webview::~webview() { }

auto webview::default_procedure(this Self& self, const pane::window_message& window_message)
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
            GetClientRect(self.window_handle(), &self.window_position.client_rect);

            FillRect(reinterpret_cast<HDC>(window_message.wparam),
                     &self.window_position.client_rect,
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

            // https://learn.microsoft.com/en-us/windows/win32/winmsg/wm-showwindow
        case WM_SHOWWINDOW: {
            if (self.controller) {
                if (window_message.wparam) {
                    self.controller->put_IsVisible(true);
                } else {
                    self.controller->put_IsVisible(false);
                }
            }

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

            if (self.controller) {
                self.controller->put_Bounds(self.window_position.client_rect);
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

auto webview::window_class_procedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> LRESULT {
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

auto webview::set_theme(this Self& self) -> void {
    auto dark_mode { pane::system::dark_mode() };

    self.window_background(dark_mode ? self.window_config.dark_background
                                     : self.window_config.light_background);

    self.window_handle.immersive_dark_mode(dark_mode);

    InvalidateRect(self.window_handle(), nullptr, true);
}

auto webview::create(this Self& self) -> HWND {
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

    if (self.options) {
        if (!self.webview_config.environment_options.AdditionalBrowserArguments.empty()) {
            self.options->put_AdditionalBrowserArguments(reinterpret_cast<const wchar_t*>(
                pane::to_utf16(self.webview_config.environment_options.AdditionalBrowserArguments)
                    .data()));
        }

        self.options->put_AllowSingleSignOnUsingOSPrimaryAccount(
            self.webview_config.environment_options.AllowSingleSignOnUsingOSPrimaryAccount);

        if (!self.webview_config.environment_options.Language.empty()) {
            self.options->put_Language(reinterpret_cast<const wchar_t*>(
                pane::to_utf16(self.webview_config.environment_options.Language).data()));
        }

        if (!self.webview_config.environment_options.TargetCompatibleBrowserVersion.empty()) {
            self.options->put_TargetCompatibleBrowserVersion(reinterpret_cast<const wchar_t*>(
                pane::to_utf16(
                    self.webview_config.environment_options.TargetCompatibleBrowserVersion)
                    .data()));
        }
    }

    if (self.options2) {
        self.options2->put_ExclusiveUserDataFolderAccess(
            self.webview_config.environment_options.ExclusiveUserDataFolderAccess);
    }

    if (self.options3) {
        self.options3->put_IsCustomCrashReportingEnabled(
            self.webview_config.environment_options.IsCustomCrashReportingEnabled);
    }

    // if (self.options4) {
    //     self.options4->SetCustomSchemeRegistrations();
    // }

    if (self.options5) {
        self.options5->put_EnableTrackingPrevention(
            self.webview_config.environment_options.EnableTrackingPrevention);
    }

    if (self.options6) {
        self.options6->put_AreBrowserExtensionsEnabled(
            self.webview_config.environment_options.AreBrowserExtensionsEnabled);
    }

    if (self.options7) {
        self.options7->put_ChannelSearchKind(
            self.webview_config.environment_options.ChannelSearchKind);
    }

    if (self.options8) {
        self.options8->put_ScrollBarStyle(self.webview_config.environment_options.ScrollBarStyle);
    }

    CreateCoreWebView2EnvironmentWithOptions(
        self.webview_config.browser_executable_folder.c_str(),
        self.webview_config.user_data_folder.c_str(),
        self.options.get(),
        wil::MakeAgileCallback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [&]([[maybe_unused]] HRESULT error_code,
                ICoreWebView2Environment* created_environment) -> HRESULT {
        if (created_environment) {
            self.environment = wil::com_ptr<ICoreWebView2Environment>(created_environment)
                                   .try_query<ICoreWebView2Environment14>();
        }

        if (self.environment) {
            self.environment->CreateCoreWebView2Controller(
                self.window_handle(),
                wil::MakeAgileCallback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                    [&]([[maybe_unused]] HRESULT error_code,
                        ICoreWebView2Controller* created_controller) -> HRESULT {
                if (created_controller) {
                    self.controller = wil::com_ptr<ICoreWebView2Controller>(created_controller)
                                          .try_query<ICoreWebView2Controller4>();
                }

                if (self.controller) {
                    self.controller->put_DefaultBackgroundColor(COREWEBVIEW2_COLOR { 0, 0, 0, 0 });

                    RECT client_rect { 0, 0, 0, 0 };
                    GetClientRect(self.window_handle(), &client_rect);
                    self.controller->put_Bounds(client_rect);

                    wil::com_ptr<ICoreWebView2> created_core;
                    self.controller->get_CoreWebView2(created_core.put());

                    if (created_core) {
                        self.core = created_core.try_query<ICoreWebView2_27>();

                        if (self.webview_config.virtual_host_name_map) {
                            const auto host_name { pane::to_utf16(
                                (*self.webview_config.virtual_host_name_map).first) };

                            self.core->SetVirtualHostNameToFolderMapping(
                                reinterpret_cast<const wchar_t*>(host_name.data()),
                                (*self.webview_config.virtual_host_name_map).second.c_str(),
                                COREWEBVIEW2_HOST_RESOURCE_ACCESS_KIND::
                                    COREWEBVIEW2_HOST_RESOURCE_ACCESS_KIND_ALLOW);
                        }
                    }

                    if (self.core) {
                        wil::com_ptr<ICoreWebView2Settings> created_settings;
                        self.core->get_Settings(created_settings.put());

                        if (created_settings) {
                            self.settings = created_settings.try_query<ICoreWebView2Settings9>();

                            if (self.settings) {
                                self.settings->put_AreBrowserAcceleratorKeysEnabled(
                                    self.webview_config.settings.AreBrowserAcceleratorKeysEnabled);
                                self.settings->put_AreDefaultContextMenusEnabled(
                                    self.webview_config.settings.AreDefaultContextMenusEnabled);
                                self.settings->put_AreDefaultScriptDialogsEnabled(
                                    self.webview_config.settings.AreDefaultScriptDialogsEnabled);
                                self.settings->put_AreDevToolsEnabled(
                                    self.webview_config.settings.AreDevToolsEnabled);
                                self.settings->put_AreHostObjectsAllowed(
                                    self.webview_config.settings.AreHostObjectsAllowed);
                                self.settings->put_HiddenPdfToolbarItems(
                                    self.webview_config.settings.HiddenPdfToolbarItems);
                                self.settings->put_IsBuiltInErrorPageEnabled(
                                    self.webview_config.settings.IsBuiltInErrorPageEnabled);
                                self.settings->put_IsGeneralAutofillEnabled(
                                    self.webview_config.settings.IsGeneralAutofillEnabled);
                                self.settings->put_IsNonClientRegionSupportEnabled(
                                    self.webview_config.settings.IsNonClientRegionSupportEnabled);
                                self.settings->put_IsPasswordAutosaveEnabled(
                                    self.webview_config.settings.IsPasswordAutosaveEnabled);
                                self.settings->put_IsPinchZoomEnabled(
                                    self.webview_config.settings.IsPinchZoomEnabled);
                                self.settings->put_IsReputationCheckingRequired(
                                    self.webview_config.settings.IsReputationCheckingRequired);
                                self.settings->put_IsScriptEnabled(
                                    self.webview_config.settings.IsScriptEnabled);
                                self.settings->put_IsStatusBarEnabled(
                                    self.webview_config.settings.IsStatusBarEnabled);
                                self.settings->put_IsSwipeNavigationEnabled(
                                    self.webview_config.settings.IsSwipeNavigationEnabled);
                                self.settings->put_IsWebMessageEnabled(
                                    self.webview_config.settings.IsWebMessageEnabled);
                                self.settings->put_IsZoomControlEnabled(
                                    self.webview_config.settings.IsZoomControlEnabled);
                            }
                        }
                    }

                    self.navigate(self.webview_config.home_page);

                    if (self.webview_config.creation_callback) {
                        self.webview_config.creation_callback(&self);
                    }
                }

                return S_OK;
            }).Get());
        }
        return S_OK;
    }).Get());

    return self.window_handle();
}

// auto webview::navigate(this const Self& self, std::u8string_view url) -> void {
//     const auto u16string { pane::to_utf16(url) };

//     if (self.core) {
//         self.core->Navigate(reinterpret_cast<const wchar_t*>(u16string.data()));
//     }
// }

auto webview::navigate(this const Self& self, const ada::url& url) -> void {
    const auto u16string { pane::to_utf16(url.get_href()) };

    if (self.core) {
        self.core->Navigate(reinterpret_cast<const wchar_t*>(u16string.data()));
    }
}

auto webview::navigate(this const Self& self, const std::filesystem::path& path) -> void {
    if (self.core) {
        self.core->Navigate(path.c_str());
    }
}

auto webview::navigate_to_string(this const Self& self, const std::u8string& string) -> void {
    const auto u16string { pane::to_utf16(string) };

    if (self.core) {
        self.core->NavigateToString(reinterpret_cast<const wchar_t*>(u16string.data()));
    }
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
