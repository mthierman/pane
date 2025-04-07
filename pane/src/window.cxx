#include <pane/window.hxx>
#include <pane/debug.hxx>

namespace pane {
window::window(std::optional<pane::window::config>&& window_config,
               std::optional<std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)>>&& window_procedure)
    : window_config { std::move(window_config.value_or(pane::window::config {})) },
      window_procedure { std::move(
          window_procedure.value_or([](HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
              return DefWindowProcW(hwnd, msg, wparam, lparam);
          })) } {
    if (GetClassInfoExW(
            this->window_class.hInstance, this->window_class.lpszClassName, &this->window_class)
        == 0) {
        RegisterClassExW(&this->window_class);
    };

    CreateWindowExW(0,
                    this->window_class.lpszClassName,
                    this->window_class.lpszClassName,
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
        this->activate();
    }

    if (this->window_config.webview) {
        this->create_webview();
    };
}

auto window::hwnd(this const Self& self) -> HWND { return self.window_handle.get(); }

auto window::activate(this const Self& self) -> bool {
    return ShowWindow(self.hwnd(), SW_SHOWNORMAL);
}

auto window::create_webview(this const Self& self) -> void {
    auto& options = self.webview.core.options;
    auto& options2 = self.webview.core.options2;
    auto& options3 = self.webview.core.options3;
    auto& options4 = self.webview.core.options4;
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

    [[maybe_unused]] auto result { CreateCoreWebView2EnvironmentWithOptions(
        browser_executable_folder.c_str(),
        user_data_folder.c_str(),
        environment_options.options.get(),
        wil::MakeAgileCallback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [this, parent_hwnd]([[maybe_unused]] HRESULT error_code,
                                ICoreWebView2Environment* created_environment) -> ::HRESULT {
        if (created_environment) {
            environment13 = wil::com_ptr<ICoreWebView2Environment>(created_environment)
                                .try_query<ICoreWebView2Environment13>();
        } else {
            // glow::log::log(errorCode);
        }

        if (environment13) {
            [[maybe_unused]] auto controllerResult { environment13->CreateCoreWebView2Controller(
                parent_hwnd,
                wil::MakeAgileCallback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                    [this]([[maybe_unused]] HRESULT error_code,
                           ICoreWebView2Controller* created_controller) -> ::HRESULT {
                if (created_controller) {
                    controller4 = wil::com_ptr<ICoreWebView2Controller>(created_controller)
                                      .try_query<ICoreWebView2Controller4>();
                } else {
                    // glow::log::log(errorCode);
                }

                if (controller4) {
                    controller4->put_DefaultBackgroundColor({ 0, 0, 0, 0 });
                    // put_bounds(client_position());

                    wil::com_ptr<ICoreWebView2> created_core;
                    controller4->get_CoreWebView2(created_core.put());

                    if (created_core) {
                        core22 = created_core.try_query<ICoreWebView2_22>();
                    }

                    if (core22) {
                        wil::com_ptr<ICoreWebView2Settings> created_settings;
                        core22->get_Settings(created_settings.put());

                        if (created_settings) {
                            settings.settings9
                                = created_settings.try_query<ICoreWebView2Settings9>();

                            if (settings.settings9) {
                                settings.settings9->put_AreBrowserAcceleratorKeysEnabled(
                                    settings.AreBrowserAcceleratorKeysEnabled);
                                settings.settings9->put_AreDefaultContextMenusEnabled(
                                    settings.AreDefaultContextMenusEnabled);
                                settings.settings9->put_AreDefaultScriptDialogsEnabled(
                                    settings.AreDefaultScriptDialogsEnabled);
                                settings.settings9->put_AreDevToolsEnabled(
                                    settings.AreDevToolsEnabled);
                                settings.settings9->put_AreHostObjectsAllowed(
                                    settings.AreHostObjectsAllowed);
                                settings.settings9->put_HiddenPdfToolbarItems(
                                    settings.HiddenPdfToolbarItems);
                                settings.settings9->put_IsBuiltInErrorPageEnabled(
                                    settings.IsBuiltInErrorPageEnabled);
                                settings.settings9->put_IsGeneralAutofillEnabled(
                                    settings.IsGeneralAutofillEnabled);
                                settings.settings9->put_IsNonClientRegionSupportEnabled(
                                    settings.IsNonClientRegionSupportEnabled);
                                settings.settings9->put_IsPasswordAutosaveEnabled(
                                    settings.IsPasswordAutosaveEnabled);
                                settings.settings9->put_IsPinchZoomEnabled(
                                    settings.IsPinchZoomEnabled);
                                settings.settings9->put_IsReputationCheckingRequired(
                                    settings.IsReputationCheckingRequired);
                                settings.settings9->put_IsScriptEnabled(settings.IsScriptEnabled);
                                settings.settings9->put_IsStatusBarEnabled(
                                    settings.IsStatusBarEnabled);
                                settings.settings9->put_IsSwipeNavigationEnabled(
                                    settings.IsSwipeNavigationEnabled);
                                settings.settings9->put_IsWebMessageEnabled(
                                    settings.IsWebMessageEnabled);
                                settings.settings9->put_IsZoomControlEnabled(
                                    settings.IsZoomControlEnabled);

                                // navigate(home_page);
                            }
                        }
                    }
                }

                return S_OK;
            }).Get()) };

            if (controllerResult != S_OK) {
                // glow::log::log(controllerResult);
            }
        }

        return S_OK;
    }).Get()) };

    // if (environmentResult != S_OK) {
    //     glow::log::log(environmentResult);
    // }
}

auto window::class_window_procedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> LRESULT {
    if (msg == WM_NCCREATE) {
        if (auto create { reinterpret_cast<CREATESTRUCTW*>(lparam) }) {
            if (auto self { static_cast<Self*>(create->lpCreateParams) }) {
                SetWindowLongPtrW(hwnd, 0, reinterpret_cast<LONG_PTR>(self));
                self->window_handle.reset(hwnd);
            }
        }
    }

    if (auto self { reinterpret_cast<Self*>(GetWindowLongPtrW(hwnd, 0)) }) {
        if (self->window_procedure) {
            return self->window_procedure(hwnd, msg, wparam, lparam);
        }
    }

    return DefWindowProcW(hwnd, msg, wparam, lparam);
}
} // namespace pane
