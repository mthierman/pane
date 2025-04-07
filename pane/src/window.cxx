#include <pane/window.hxx>
#include <pane/debug.hxx>

namespace pane {
window::window(std::optional<pane::window::config>&& window_config,
               std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)>&& window_procedure)
    : window_config { std::move(window_config.value_or(pane::window::config {})) },
      window_procedure { std::move(window_procedure) } { }

auto window::activate(this Self& self) -> bool {
    if (GetClassInfoExW(
            self.window_class.hInstance, self.window_class.lpszClassName, &self.window_class)
        == 0) {
        RegisterClassExW(&self.window_class);
    };

    CreateWindowExW(0,
                    self.window_class.lpszClassName,
                    self.window_class.lpszClassName,
                    WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
                    CW_USEDEFAULT,
                    CW_USEDEFAULT,
                    CW_USEDEFAULT,
                    CW_USEDEFAULT,
                    nullptr,
                    nullptr,
                    self.window_class.hInstance,
                    &self);

    return ShowWindow(self.window_handle, SW_SHOWNORMAL);
}

auto window::create_webview(this Self& self) -> void {
    auto& options = self.webview.core.options;
    auto& options2 = self.webview.core.options2;
    auto& options3 = self.webview.core.options3;
    // auto& options4 = self.webview.core.options4;
    auto& options5 = self.webview.core.options5;
    auto& options6 = self.webview.core.options6;
    auto& options7 = self.webview.core.options7;
    auto& options8 = self.webview.core.options8;

    auto& environment_options = self.webview.environment_options;

    if (options) {
        if (!environment_options.AdditionalBrowserArguments.empty()) {
            // if (auto converted {
            //         glow::text::u16string(config.environmentOptions.AdditionalBrowserArguments)
            //         };
            //     converted) {
            //     createdEnvironmentOptions->put_AdditionalBrowserArguments(
            //         glow::text::c_str(converted.value()));
            // }
        }

        options->put_AllowSingleSignOnUsingOSPrimaryAccount(
            environment_options.AllowSingleSignOnUsingOSPrimaryAccount);

        if (!environment_options.Language.empty()) {
            // if (auto converted { glow::text::u16string(config.environmentOptions.Language) };
            //     converted) {
            //     createdEnvironmentOptions->put_Language(glow::text::c_str(converted.value()));
            // }
        }

        if (!environment_options.TargetCompatibleBrowserVersion.empty()) {
            // if (auto converted { glow::text::u16string(
            //         config.environmentOptions.TargetCompatibleBrowserVersion) };
            //     converted) {
            //     createdEnvironmentOptions->put_TargetCompatibleBrowserVersion(
            //         glow::text::c_str(converted.value()));
            // }
        }
    }

    if (options2) {
        options2->put_ExclusiveUserDataFolderAccess(
            environment_options.ExclusiveUserDataFolderAccess);
    }

    if (options3) {
        options3->put_IsCustomCrashReportingEnabled(
            environment_options.IsCustomCrashReportingEnabled);
    }

    // if (options4) {
    //     options4->SetCustomSchemeRegistrations();
    // }

    if (options5) {
        options5->put_EnableTrackingPrevention(environment_options.EnableTrackingPrevention);
    }

    if (options6) {
        options6->put_AreBrowserExtensionsEnabled(environment_options.AreBrowserExtensionsEnabled);
    }

    if (options7) {
        options7->put_ChannelSearchKind(environment_options.ChannelSearchKind);
    }

    if (options8) {
        options8->put_ScrollBarStyle(environment_options.ScrollBarStyle);
    }

    CreateCoreWebView2EnvironmentWithOptions(
        self.webview.browser_executable_folder.c_str(),
        self.webview.user_data_folder.c_str(),
        options.get(),
        wil::MakeAgileCallback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [&self]([[maybe_unused]] HRESULT error_code,
                    ICoreWebView2Environment* created_environment) -> HRESULT {
        if (created_environment) {
            self.webview.core.environment13
                = wil::com_ptr<ICoreWebView2Environment>(created_environment)
                      .try_query<ICoreWebView2Environment13>();
        }

        if (self.webview.core.environment13) {
            self.webview.core.environment13->CreateCoreWebView2Controller(
                self.window_handle,
                wil::MakeAgileCallback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                    [&self]([[maybe_unused]] HRESULT error_code,
                            ICoreWebView2Controller* created_controller) -> ::HRESULT {
                if (created_controller) {
                    self.webview.core.controller4
                        = wil::com_ptr<ICoreWebView2Controller>(created_controller)
                              .try_query<ICoreWebView2Controller4>();
                }

                if (self.webview.core.controller4) {
                    self.webview.core.controller4->put_DefaultBackgroundColor({ 0, 0, 0, 0 });
                    // put_bounds(client_position());
                    self.webview.core.controller4->put_Bounds({ 0, 0, 300, 300 });

                    wil::com_ptr<ICoreWebView2> created_core;
                    self.webview.core.controller4->get_CoreWebView2(created_core.put());

                    if (created_core) {
                        self.webview.core.core22 = created_core.try_query<ICoreWebView2_22>();
                    }

                    if (self.webview.core.core22) {
                        wil::com_ptr<ICoreWebView2Settings> created_settings;
                        self.webview.core.core22->get_Settings(created_settings.put());

                        if (created_settings) {
                            self.webview.core.settings9
                                = created_settings.try_query<ICoreWebView2Settings9>();

                            if (self.webview.core.settings9) {
                                auto& settings9 { self.webview.core.settings9 };
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

                                // navigate(home_page);
                                self.webview.core.core22->Navigate(L"https://www.google.com/");
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

auto window::class_window_procedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> LRESULT {
    if (msg == WM_NCCREATE) {
        if (auto create { reinterpret_cast<CREATESTRUCTW*>(lparam) }) {
            if (auto self { static_cast<Self*>(create->lpCreateParams) }) {
                SetWindowLongPtrW(hwnd, 0, reinterpret_cast<LONG_PTR>(self));
                self->window_handle = hwnd;
            }
        }
    } else {
        if (auto self { reinterpret_cast<Self*>(GetWindowLongPtrW(hwnd, 0)) }) {
            if (self->window_procedure) {
                return self->window_procedure(hwnd, msg, wparam, lparam);
            }
        }
    }

    return DefWindowProcW(hwnd, msg, wparam, lparam);
}
} // namespace pane
