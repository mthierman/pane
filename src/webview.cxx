#include <pane/input.hxx>
#include <pane/webview.hxx>
#include <pane/text.hxx>
#include <dwmapi.h>
#include <wil/wrl.h>

#include <pane/debug.hxx>

namespace pane {
auto webview_token::operator()(this Self& self) -> EventRegistrationToken* { return &self.token; }

webview::webview(pane::window_config&& window_config,
                 pane::webview_config&& webview_config,
                 std::function<LRESULT(Self*, pane::window_message)>&& window_procedure)
    : window_procedure { std::move(window_procedure) },
      window_config { std::move(window_config) },
      webview_config { std::move(webview_config) } {
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

    if (self.environment_options) {
        if (!self.webview_config.environment_options.AdditionalBrowserArguments.empty()) {
            self.environment_options->put_AdditionalBrowserArguments(
                reinterpret_cast<const wchar_t*>(
                    pane::to_utf16(
                        self.webview_config.environment_options.AdditionalBrowserArguments)
                        .data()));
        }

        self.environment_options->put_AllowSingleSignOnUsingOSPrimaryAccount(
            self.webview_config.environment_options.AllowSingleSignOnUsingOSPrimaryAccount);

        if (!self.webview_config.environment_options.Language.empty()) {
            self.environment_options->put_Language(reinterpret_cast<const wchar_t*>(
                pane::to_utf16(self.webview_config.environment_options.Language).data()));
        }

        if (!self.webview_config.environment_options.TargetCompatibleBrowserVersion.empty()) {
            self.environment_options->put_TargetCompatibleBrowserVersion(
                reinterpret_cast<const wchar_t*>(
                    pane::to_utf16(
                        self.webview_config.environment_options.TargetCompatibleBrowserVersion)
                        .data()));
        }
    }

    if (self.environment_options2) {
        self.environment_options2->put_ExclusiveUserDataFolderAccess(
            self.webview_config.environment_options.ExclusiveUserDataFolderAccess);
    }

    if (self.environment_options3) {
        self.environment_options3->put_IsCustomCrashReportingEnabled(
            self.webview_config.environment_options.IsCustomCrashReportingEnabled);
    }

    // if (self.environment_options4) {
    //     self.environment_options4->SetCustomSchemeRegistrations();
    // }

    if (self.environment_options5) {
        self.environment_options5->put_EnableTrackingPrevention(
            self.webview_config.environment_options.EnableTrackingPrevention);
    }

    if (self.environment_options6) {
        self.environment_options6->put_AreBrowserExtensionsEnabled(
            self.webview_config.environment_options.AreBrowserExtensionsEnabled);
    }

    if (self.environment_options7) {
        self.environment_options7->put_ChannelSearchKind(
            self.webview_config.environment_options.ChannelSearchKind);
    }

    if (self.environment_options8) {
        self.environment_options8->put_ScrollBarStyle(
            self.webview_config.environment_options.ScrollBarStyle);
    }

    CreateCoreWebView2EnvironmentWithOptions(
        self.webview_config.browser_executable_folder.c_str(),
        self.webview_config.user_data_folder.c_str(),
        self.environment_options.get(),
        wil::MakeAgileCallback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [&]([[maybe_unused]] HRESULT error_code,
                ICoreWebView2Environment* created_environment) -> HRESULT {
        if (created_environment) {
            self.environment = wil::com_ptr<ICoreWebView2Environment>(created_environment)
                                   .try_query<ICoreWebView2Environment14>();
        }

        if (self.environment) {
            wil::com_ptr<ICoreWebView2ControllerOptions> controller_options;
            self.environment->CreateCoreWebView2ControllerOptions(controller_options.addressof());
            self.controller_options
                = controller_options.try_query<ICoreWebView2ControllerOptions4>();

            if (self.controller_options) {
                self.controller_options->put_AllowHostInputProcessing(TRUE);
                self.controller_options->put_DefaultBackgroundColor(
                    COREWEBVIEW2_COLOR { 0, 0, 0, 0 });
            }

            self.environment->CreateCoreWebView2ControllerWithOptions(
                self.window_handle(),
                self.controller_options.get(),
                wil::MakeAgileCallback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                    [&]([[maybe_unused]] HRESULT error_code,
                        ICoreWebView2Controller* created_controller) -> HRESULT {
                if (created_controller) {
                    self.controller = wil::com_ptr<ICoreWebView2Controller>(created_controller)
                                          .try_query<ICoreWebView2Controller4>();
                }

                if (self.controller) {
                    RECT client_rect { 0, 0, 0, 0 };
                    GetClientRect(self.window_handle(), &client_rect);
                    self.controller->put_Bounds(client_rect);

                    self.controller->add_AcceleratorKeyPressed(
                        Microsoft::WRL::Callback<ICoreWebView2AcceleratorKeyPressedEventHandler>(
                            [&](ICoreWebView2Controller* sender,
                                ICoreWebView2AcceleratorKeyPressedEventArgs* args) -> HRESULT {
                        UINT key;
                        args->get_VirtualKey(&key);

                        COREWEBVIEW2_PHYSICAL_KEY_STATUS key_status;
                        args->get_PhysicalKeyStatus(&key_status);

                        if (key == VK_F11) {
                            if (!key_status.WasKeyDown) {
                                self.window_handle.toggle_fullscreen();
                            }
                        }

                        return S_OK;
                    }).Get(),
                        self.token.accelerator_key_pressed());

                    wil::com_ptr<ICoreWebView2> created_core;
                    self.controller->get_CoreWebView2(created_core.put());

                    if (created_core) {
                        self.core = created_core.try_query<ICoreWebView2_27>();
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

                        if (self.webview_config.virtual_host_name_map) {
                            const auto host_name { pane::to_utf16(
                                (*self.webview_config.virtual_host_name_map).first) };

                            self.core->SetVirtualHostNameToFolderMapping(
                                reinterpret_cast<const wchar_t*>(host_name.data()),
                                (*self.webview_config.virtual_host_name_map).second.c_str(),
                                COREWEBVIEW2_HOST_RESOURCE_ACCESS_KIND::
                                    COREWEBVIEW2_HOST_RESOURCE_ACCESS_KIND_ALLOW);
                        }

                        self.core->add_FaviconChanged(
                            Microsoft::WRL::Callback<ICoreWebView2FaviconChangedEventHandler>(
                                [&](ICoreWebView2* /* sender */, IUnknown* /* args */) -> HRESULT {
                            self.core->GetFavicon(
                                COREWEBVIEW2_FAVICON_IMAGE_FORMAT::
                                    COREWEBVIEW2_FAVICON_IMAGE_FORMAT_PNG,
                                Microsoft::WRL::Callback<ICoreWebView2GetFaviconCompletedHandler>(
                                    [&](HRESULT /* error_code */, IStream* icon_stream) -> HRESULT {
                                self.favicon_status
                                    = Gdiplus::Bitmap { icon_stream }.GetHICON(&self.favicon());

                                return S_OK;
                            }).Get());

                            return S_OK;
                        }).Get(),
                            self.token.favicon_changed());

                        self.core->add_NavigationCompleted(
                            Microsoft::WRL::Callback<ICoreWebView2NavigationCompletedEventHandler>(
                                [&](ICoreWebView2* /* sender */,
                                    ICoreWebView2NavigationCompletedEventArgs* /* args */)
                                    -> HRESULT {
                            wil::unique_cotaskmem_string title;
                            self.core->get_DocumentTitle(&title);
                            self.current_title = pane::to_utf8(title.get());

                            return S_OK;
                        }).Get(),
                            self.token.source_changed());

                        self.navigate(self.webview_config.home_page);

                        if (self.webview_config.creation_callback) {
                            self.webview_config.creation_callback(&self);
                        }
                    }
                }

                return S_OK;
            }).Get());
        }
        return S_OK;
    }).Get());
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

// auto webview::navigate(this const Self& self, std::u8string_view url) -> void {
//     const auto u16string { pane::to_utf16(url) };

//     if (self.core) {
//         self.core->Navigate(reinterpret_cast<const wchar_t*>(u16string.data()));
//     }
// }

auto webview::navigate(this Self& self, const ada::url& url) -> void {
    self.current_url = url;
    const auto u16string { pane::to_utf16(url.get_href()) };

    if (self.core) {
        self.core->Navigate(reinterpret_cast<const wchar_t*>(u16string.data()));
    }
}

auto webview::navigate(this Self& self, const std::filesystem::path& path) -> void {
    if (auto parse { ada::parse<ada::url>(path.string()) }; parse) {
        self.current_url = parse.value();
    }

    if (self.core) {
        self.core->Navigate(path.c_str());
    }
}

auto webview::navigate_to_string(this Self& self, const std::u8string& string) -> void {
    if (auto parse { ada::parse<ada::url>(reinterpret_cast<const char*>(string.c_str())) }; parse) {
        self.current_url = parse.value();
    }

    const auto u16string { pane::to_utf16(string) };

    if (self.core) {
        self.core->NavigateToString(reinterpret_cast<const wchar_t*>(u16string.c_str()));
    }
}
} // namespace pane
