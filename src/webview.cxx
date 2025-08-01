#include <pane/webview.hxx>
#include <pane/text.hxx>
#include <dwmapi.h>
#include <wil/wrl.h>

namespace pane {
auto webview_token::operator()(this Self& self) -> EventRegistrationToken* { return &self.token; }

webview::webview(struct window_config window_config,
                 struct webview_config webview_config,
                 procedure_fn window_procedure)
    : window_config { std::move(window_config) },
      webview_config { std::move(webview_config) },
      window_procedure { std::move(window_procedure) } {
    auto& self = *this;

    window_class<Self> window_class { u8"pane_webview" };

    CreateWindowExW(0,
                    window_class.data.lpszClassName,
                    reinterpret_cast<const wchar_t*>(to_utf16(self.window_config.title).data()),
                    self.window_config.parent_hwnd
                        ? WS_CHILDWINDOW
                        : WS_OVERLAPPEDWINDOW | (self.window_config.visible ? WS_VISIBLE : 0),
                    CW_USEDEFAULT,
                    CW_USEDEFAULT,
                    CW_USEDEFAULT,
                    CW_USEDEFAULT,
                    self.window_config.parent_hwnd,
                    self.window_config.parent_hwnd ? reinterpret_cast<HMENU>(self.window_handle.id)
                                                   : nullptr,
                    window_class.data.hInstance,
                    &self);

    if (self.environment_options) {
        if (!self.webview_config.environment_options.AdditionalBrowserArguments.empty()) {
            self.environment_options->put_AdditionalBrowserArguments(
                reinterpret_cast<const wchar_t*>(
                    to_utf16(self.webview_config.environment_options.AdditionalBrowserArguments)
                        .data()));
        }

        self.environment_options->put_AllowSingleSignOnUsingOSPrimaryAccount(
            self.webview_config.environment_options.AllowSingleSignOnUsingOSPrimaryAccount);

        if (!self.webview_config.environment_options.Language.empty()) {
            self.environment_options->put_Language(reinterpret_cast<const wchar_t*>(
                to_utf16(self.webview_config.environment_options.Language).data()));
        }

        if (!self.webview_config.environment_options.TargetCompatibleBrowserVersion.empty()) {
            self.environment_options->put_TargetCompatibleBrowserVersion(
                reinterpret_cast<const wchar_t*>(
                    to_utf16(self.webview_config.environment_options.TargetCompatibleBrowserVersion)
                        .data()));
        }

        if (auto environment_options2 {
                self.environment_options.try_query<ICoreWebView2EnvironmentOptions2>() }) {
            environment_options2->put_ExclusiveUserDataFolderAccess(
                self.webview_config.environment_options.ExclusiveUserDataFolderAccess);
        }

        if (auto environment_options3 {
                self.environment_options.try_query<ICoreWebView2EnvironmentOptions3>() }) {
            environment_options3->put_IsCustomCrashReportingEnabled(
                self.webview_config.environment_options.IsCustomCrashReportingEnabled);
        }

        // if (auto environment_options4 {
        //         self.environment_options.try_query<ICoreWebView2EnvironmentOptions4>() }) {
        //     environment_options4->SetCustomSchemeRegistrations();
        // }

        if (auto environment_options5 {
                self.environment_options.try_query<ICoreWebView2EnvironmentOptions5>() }) {
            environment_options5->put_EnableTrackingPrevention(
                self.webview_config.environment_options.EnableTrackingPrevention);
        }

        if (auto environment_options6 {
                self.environment_options.try_query<ICoreWebView2EnvironmentOptions6>() }) {
            environment_options6->put_AreBrowserExtensionsEnabled(
                self.webview_config.environment_options.AreBrowserExtensionsEnabled);
        }

        if (auto environment_options7 {
                self.environment_options.try_query<ICoreWebView2EnvironmentOptions7>() }) {
            environment_options7->put_ChannelSearchKind(
                self.webview_config.environment_options.ChannelSearchKind);
        }

        if (auto environment_options8 {
                self.environment_options.try_query<ICoreWebView2EnvironmentOptions8>() }) {
            environment_options8->put_ScrollBarStyle(
                self.webview_config.environment_options.ScrollBarStyle);
        }

        CreateCoreWebView2EnvironmentWithOptions(
            self.webview_config.browser_executable_folder.c_str(),
            self.webview_config.user_data_folder.c_str(),
            self.environment_options.get(),
            wil::MakeAgileCallback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
                [&](HRESULT /* error_code */,
                    ICoreWebView2Environment* created_environment) -> HRESULT {
            if (created_environment) {
                self.environment = wil::com_ptr<ICoreWebView2Environment>(created_environment)
                                       .try_query<ICoreWebView2Environment14>();
            }

            if (self.environment) {
                wil::com_ptr<ICoreWebView2ControllerOptions> controller_options;
                self.environment->CreateCoreWebView2ControllerOptions(controller_options.put());

                if (controller_options) {
                    self.controller_options
                        = controller_options.try_query<ICoreWebView2ControllerOptions4>();
                }

                if (self.controller_options) {
                    self.controller_options->put_AllowHostInputProcessing(TRUE);
                    self.controller_options->put_DefaultBackgroundColor(
                        COREWEBVIEW2_COLOR { 0, 0, 0, 0 });
                }

                self.environment->CreateCoreWebView2ControllerWithOptions(
                    self.window_handle(),
                    self.controller_options.get(),
                    wil::MakeAgileCallback<
                        ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                        [&](HRESULT /* error_code */,
                            ICoreWebView2Controller* created_controller) -> HRESULT {
                    if (created_controller) {
                        self.controller = wil::com_ptr<ICoreWebView2Controller>(created_controller)
                                              .try_query<ICoreWebView2Controller4>();
                    }

                    if (self.controller) {
                        RECT client_rect { 0, 0, 0, 0 };
                        GetClientRect(self.window_handle(), &client_rect);
                        self.controller->put_Bounds(client_rect);

                        // https://learn.microsoft.com/en-us/microsoft-edge/webview2/reference/win32/icorewebview2controller?view=webview2-1.0.3351.48#add_acceleratorkeypressed
                        self.controller->add_AcceleratorKeyPressed(
                            Microsoft::WRL::Callback<
                                ICoreWebView2AcceleratorKeyPressedEventHandler>(
                                [&](ICoreWebView2Controller* /* sender */,
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
                                self.settings
                                    = created_settings.try_query<ICoreWebView2Settings9>();

                                if (self.settings) {
                                    self.settings->put_AreBrowserAcceleratorKeysEnabled(
                                        self.webview_config.settings
                                            .AreBrowserAcceleratorKeysEnabled);
                                    self.settings->put_AreDefaultContextMenusEnabled(
                                        self.webview_config.settings.AreDefaultContextMenusEnabled);
                                    self.settings->put_AreDefaultScriptDialogsEnabled(
                                        self.webview_config.settings
                                            .AreDefaultScriptDialogsEnabled);
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
                                        self.webview_config.settings
                                            .IsNonClientRegionSupportEnabled);
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
                                const auto host_name { to_utf16(
                                    (*self.webview_config.virtual_host_name_map).first) };

                                self.core->SetVirtualHostNameToFolderMapping(
                                    reinterpret_cast<const wchar_t*>(host_name.data()),
                                    (*self.webview_config.virtual_host_name_map).second.c_str(),
                                    COREWEBVIEW2_HOST_RESOURCE_ACCESS_KIND::
                                        COREWEBVIEW2_HOST_RESOURCE_ACCESS_KIND_ALLOW);
                            }

                            self.core->add_FaviconChanged(
                                Microsoft::WRL::Callback<ICoreWebView2FaviconChangedEventHandler>(
                                    [&](ICoreWebView2* /* sender */,
                                        IUnknown* /* args */) -> HRESULT {
                                self.core->GetFavicon(COREWEBVIEW2_FAVICON_IMAGE_FORMAT::
                                                          COREWEBVIEW2_FAVICON_IMAGE_FORMAT_PNG,
                                                      Microsoft::WRL::Callback<
                                                          ICoreWebView2GetFaviconCompletedHandler>(
                                                          [&](HRESULT /* error_code */,
                                                              IStream* icon_stream) -> HRESULT {
                                    self.favicon_status
                                        = Gdiplus::Bitmap { icon_stream }.GetHICON(&self.favicon());

                                    make_window_message(self.window_handle(),
                                                        message::FAVICON_CHANGED,
                                                        0,
                                                        self.favicon())
                                        .send();

                                    return S_OK;
                                }).Get());

                                return S_OK;
                            }).Get(),
                                self.token.favicon_changed());

                            self.core->add_NavigationCompleted(
                                Microsoft::WRL::Callback<
                                    ICoreWebView2NavigationCompletedEventHandler>(
                                    [&](ICoreWebView2* /* sender */,
                                        ICoreWebView2NavigationCompletedEventArgs* /* args */)
                                        -> HRESULT {
                                wil::unique_cotaskmem_string title;
                                self.core->get_DocumentTitle(&title);
                                self.current_title = to_utf8(title.get());

                                make_window_message(
                                    self.window_handle(), message::NAVIGATION_COMPLETED, 0, 0)
                                    .send();

                                return S_OK;
                            }).Get(),
                                self.token.navigation_completed());

                            self.navigate(self.webview_config.home_page);

                            make_window_message(
                                self.window_handle(), message::WEBVIEW_CREATE, 0, self.favicon())
                                .send();
                        }
                    }

                    return S_OK;
                }).Get());
            }
            return S_OK;
        }).Get());
    }
}

auto webview::default_procedure(this Self& self, const window_message& window_message) -> LRESULT {
    switch (window_message.msg) {
        case WM_CLOSE: {
            // TODO: FIX CRASHING HERE CALLING TO_UTF16 (string length/resize problem)
            // TODO UPDATE: THIS IS ALL CRASHING BECAUSE WE ARE DESTROYING WINDOW FIRST IN CALLBACK
            // PROCEDURE!
            //
            // if (self.webview_config.virtual_host_name_map) {
            //     self.core->ClearVirtualHostNameToFolderMapping(reinterpret_cast<const wchar_t*>(
            //         to_utf16((*self.webview_config.virtual_host_name_map).first).data()));
            // }

            // self.controller->remove_AcceleratorKeyPressed(*self.token.accelerator_key_pressed());
            // self.core->remove_FaviconChanged(*self.token.favicon_changed());
            // self.core->remove_NavigationCompleted(*self.token.navigation_completed());
        } break;

        // https://learn.microsoft.com/en-us/windows/win32/hidpi/wm-dpichanged
        case WM_DPICHANGED: {
            auto const suggested_rect { reinterpret_cast<RECT*>(window_message.lparam) };
            SetWindowPos(window_message.hwnd,
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
            GetClientRect(window_message.hwnd, &self.window_handle.position.client_rect);

            FillRect(reinterpret_cast<HDC>(window_message.wparam),
                     &self.window_handle.position.client_rect,
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

            // https://learn.microsoft.com/en-us/windows/win32/winmsg/wm-showwindow
        case WM_SHOWWINDOW: {
            if (self.controller) {
                self.controller->put_IsVisible(window_message.wparam);
            }

            return 0;
        } break;

            // https://learn.microsoft.com/en-us/windows/win32/winmsg/wm-windowposchanged
        case WM_WINDOWPOSCHANGED: {
            if (self.controller) {
                self.controller->put_Bounds(self.window_handle.position.client_rect);
            }
        } break;
    }

    return window_message.default_procedure();
}

// auto webview::navigate(this const Self& self, std::u8string_view url) -> void {
//     const auto u16string { to_utf16(url) };

//     if (self.core) {
//         self.core->Navigate(reinterpret_cast<const wchar_t*>(u16string.data()));
//     }
// }

auto webview::navigate(this Self& self, const ada::url& url) -> void {
    self.current_url = url;
    const auto u16string { to_utf16(url.get_href()) };

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

    const auto u16string { to_utf16(string) };

    if (self.core) {
        self.core->NavigateToString(reinterpret_cast<const wchar_t*>(u16string.c_str()));
    }
}
} // namespace pane
