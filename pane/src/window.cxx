#include <pane/window.hxx>
#include <pane/text.hxx>
#include <pane/debug.hxx>

namespace pane {
window::window(pane::window::config&& window_config,
               std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)>&& window_procedure)
    : window_config { std::move(window_config) },
      window_brush { window_config.background_color.to_hbrush() },
      window_procedure { std::move(window_procedure) } {
    if (GetClassInfoExW(
            this->window_class.hInstance, this->window_class.lpszClassName, &this->window_class)
        == 0) {
        RegisterClassExW(&this->window_class);
    };

    CreateWindowExW(
        0,
        this->window_class.lpszClassName,
        reinterpret_cast<const wchar_t*>(pane::to_utf16(this->window_config.title).data()),
        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        nullptr,
        nullptr,
        this->window_class.hInstance,
        this);

    if (this->window_config.visible) {
        ShowWindow(this->window_handle.get(), SW_SHOWNORMAL);
    }

    if (this->window_config.webview) {
        this->create_webview();
    }
}

auto window::create_webview(this Self& self) -> void {
    if (self.webview.core_options) {
        if (!self.webview.environment_options.AdditionalBrowserArguments.empty()) {
            self.webview.core_options->put_AdditionalBrowserArguments(
                reinterpret_cast<const wchar_t*>(
                    pane::to_utf16(self.webview.environment_options.AdditionalBrowserArguments)
                        .data()));
        }

        self.webview.core_options->put_AllowSingleSignOnUsingOSPrimaryAccount(
            self.webview.environment_options.AllowSingleSignOnUsingOSPrimaryAccount);

        if (!self.webview.environment_options.Language.empty()) {
            self.webview.core_options->put_Language(reinterpret_cast<const wchar_t*>(
                pane::to_utf16(self.webview.environment_options.Language).data()));
        }

        if (!self.webview.environment_options.TargetCompatibleBrowserVersion.empty()) {
            self.webview.core_options->put_TargetCompatibleBrowserVersion(
                reinterpret_cast<const wchar_t*>(
                    pane::to_utf16(self.webview.environment_options.TargetCompatibleBrowserVersion)
                        .data()));
        }
    }

    if (self.webview.core_options2) {
        self.webview.core_options2->put_ExclusiveUserDataFolderAccess(
            self.webview.environment_options.ExclusiveUserDataFolderAccess);
    }

    if (self.webview.core_options3) {
        self.webview.core_options3->put_IsCustomCrashReportingEnabled(
            self.webview.environment_options.IsCustomCrashReportingEnabled);
    }

    // if (self.webview.webview_options4) {
    //     self.webview.webview_options4->SetCustomSchemeRegistrations();
    // }

    if (self.webview.core_options5) {
        self.webview.core_options5->put_EnableTrackingPrevention(
            self.webview.environment_options.EnableTrackingPrevention);
    }

    if (self.webview.core_options6) {
        self.webview.core_options6->put_AreBrowserExtensionsEnabled(
            self.webview.environment_options.AreBrowserExtensionsEnabled);
    }

    if (self.webview.core_options7) {
        self.webview.core_options7->put_ChannelSearchKind(
            self.webview.environment_options.ChannelSearchKind);
    }

    if (self.webview.core_options8) {
        self.webview.core_options8->put_ScrollBarStyle(
            self.webview.environment_options.ScrollBarStyle);
    }

    CreateCoreWebView2EnvironmentWithOptions(
        self.webview.browser_executable_folder.c_str(),
        self.webview.user_data_folder.c_str(),
        self.webview.core_options.get(),
        wil::MakeAgileCallback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [&self]([[maybe_unused]] HRESULT error_code,
                    ICoreWebView2Environment* created_environment) -> HRESULT {
        if (created_environment) {
            self.webview.core_environment
                = wil::com_ptr<ICoreWebView2Environment>(created_environment)
                      .try_query<ICoreWebView2Environment13>();
        }

        if (self.webview.core_environment) {
            self.webview.core_environment->CreateCoreWebView2Controller(
                self.window_handle.get(),
                wil::MakeAgileCallback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                    [&self]([[maybe_unused]] HRESULT error_code,
                            ICoreWebView2Controller* created_controller) -> HRESULT {
                if (created_controller) {
                    self.webview.core_controller
                        = wil::com_ptr<ICoreWebView2Controller>(created_controller)
                              .try_query<ICoreWebView2Controller4>();
                }

                if (self.webview.core_controller) {
                    self.webview.core_controller->put_DefaultBackgroundColor(
                        self.window_config.background_color.to_webview2_color());

                    RECT client_rect {};
                    GetClientRect(self.window_handle.get(), &client_rect);

                    self.webview.core_controller->put_Bounds(client_rect);

                    wil::com_ptr<ICoreWebView2> created_core;
                    self.webview.core_controller->get_CoreWebView2(created_core.put());

                    if (created_core) {
                        self.webview.core_core = created_core.try_query<ICoreWebView2_22>();
                    }

                    if (self.webview.core_core) {
                        wil::com_ptr<ICoreWebView2Settings> created_settings;
                        self.webview.core_core->get_Settings(created_settings.put());

                        if (created_settings) {
                            self.webview.core_settings
                                = created_settings.try_query<ICoreWebView2Settings9>();

                            if (self.webview.core_settings) {
                                auto& settings9 { self.webview.core_settings };
                                auto& settings { self.webview.settings };

                                settings9->put_AreBrowserAcceleratorKeysEnabled(
                                    settings.AreBrowserAcceleratorKeysEnabled);
                                settings9->put_AreDefaultContextMenusEnabled(
                                    settings.AreDefaultContextMenusEnabled);
                                settings9->put_AreDefaultScriptDialogsEnabled(
                                    settings.AreDefaultScriptDialogsEnabled);
                                settings9->put_AreDevToolsEnabled(settings.AreDevToolsEnabled);
                                settings9->put_AreHostObjectsAllowed(
                                    settings.AreHostObjectsAllowed);
                                settings9->put_HiddenPdfToolbarItems(
                                    settings.HiddenPdfToolbarItems);
                                settings9->put_IsBuiltInErrorPageEnabled(
                                    settings.IsBuiltInErrorPageEnabled);
                                settings9->put_IsGeneralAutofillEnabled(
                                    settings.IsGeneralAutofillEnabled);
                                settings9->put_IsNonClientRegionSupportEnabled(
                                    settings.IsNonClientRegionSupportEnabled);
                                settings9->put_IsPasswordAutosaveEnabled(
                                    settings.IsPasswordAutosaveEnabled);
                                settings9->put_IsPinchZoomEnabled(settings.IsPinchZoomEnabled);
                                settings9->put_IsReputationCheckingRequired(
                                    settings.IsReputationCheckingRequired);
                                settings9->put_IsScriptEnabled(settings.IsScriptEnabled);
                                settings9->put_IsStatusBarEnabled(settings.IsStatusBarEnabled);
                                settings9->put_IsSwipeNavigationEnabled(
                                    settings.IsSwipeNavigationEnabled);
                                settings9->put_IsWebMessageEnabled(settings.IsWebMessageEnabled);
                                settings9->put_IsZoomControlEnabled(settings.IsZoomControlEnabled);

                                self.navigate(self.webview.home_page);
                            }
                        }
                    }
                }

                return S_OK;
            }).Get());
        }
        return S_OK;
    }).Get());
}

auto window::navigate(this Self& self, std::u8string_view url) -> void {
    if (self.webview.core_core) {
        self.webview.core_core->Navigate(
            reinterpret_cast<const wchar_t*>(pane::to_utf16(url).data()));
    }
}

auto window::class_window_procedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> LRESULT {
    if (msg == WM_NCCREATE) {
        if (auto create { reinterpret_cast<CREATESTRUCTW*>(lparam) }) {
            if (auto self { static_cast<Self*>(create->lpCreateParams) }) {
                SetWindowLongPtrW(hwnd, 0, reinterpret_cast<LONG_PTR>(self));
                self->window_handle.reset(hwnd);
            }
        }
    } else {
        if (auto self { reinterpret_cast<Self*>(GetWindowLongPtrW(hwnd, 0)) }) {
            switch (msg) {
                case WM_WINDOWPOSCHANGED: {
                    RECT client_rect {};
                    GetClientRect(hwnd, &client_rect);

                    if (self->webview.core_controller) {
                        self->webview.core_controller->put_Bounds(client_rect);
                    }
                } break;
                case WM_ERASEBKGND: {
                    RECT client_rect {};
                    GetClientRect(hwnd, &client_rect);
                    FillRect(reinterpret_cast<HDC>(wparam), &client_rect, self->window_brush.get());
                }
            }

            if (self->window_procedure) {
                return self->window_procedure(hwnd, msg, wparam, lparam);
            }
        }
    }

    return DefWindowProcW(hwnd, msg, wparam, lparam);
}
} // namespace pane
