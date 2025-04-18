#include <pane/window.hxx>
#include <pane/text.hxx>
#include <pane/debug.hxx>

namespace pane {
window::window(pane::window_config&& window_config,
               std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)>&& window_procedure)
    : window_config { std::move(window_config) },
      window_procedure { std::move(window_procedure) } {
    this->window_class.hbrBackground = this->window_config.background_color.to_hbrush();

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
        this->activate();
    }
}

window::~window() {
    UnregisterClassW(this->window_class.lpszClassName, this->window_class.hInstance);
}

auto window::activate(this const Self& self) -> bool {
    return ShowWindow(self.window_handle, SW_SHOWNORMAL);
}

auto window::show(this const Self& self) -> bool { return ShowWindow(self.window_handle, SW_SHOW); }

auto window::hide(this const Self& self) -> bool { return ShowWindow(self.window_handle, SW_HIDE); }

auto window::client_rect(this const Self& self) -> RECT {
    RECT client_rect {};
    GetClientRect(self.window_handle, &client_rect);

    return client_rect;
}

auto window::get_instance(HWND hwnd) -> Self* {
    return reinterpret_cast<Self*>(GetWindowLongPtrW(hwnd, 0));
}

auto window::class_window_procedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> LRESULT {
    if (msg == WM_NCCREATE) {
        if (auto create { reinterpret_cast<CREATESTRUCTW*>(lparam) }) {
            if (auto self { static_cast<Self*>(create->lpCreateParams) }) {
                SetWindowLongPtrW(hwnd, 0, reinterpret_cast<LONG_PTR>(self));
                self->window_handle = hwnd;
            }
        }
    }

    if (auto self { reinterpret_cast<Self*>(GetWindowLongPtrW(hwnd, 0)) }) {
        if (msg == WM_NCDESTROY) {
            SetWindowLongPtrW(hwnd, 0, reinterpret_cast<LONG_PTR>(nullptr));
            self->window_handle = nullptr;
        }

        if (msg == WM_WINDOWPOSCHANGED) {
            // auto client_rect { self->client_rect() };

            // if (self->webview.controller) {
            //     self->webview.controller->put_Bounds(client_rect);
            // }
        }

        if (msg == WM_ERASEBKGND) {
            auto client_rect { self->client_rect() };
            FillRect(reinterpret_cast<HDC>(wparam), &client_rect, self->window_class.hbrBackground);
        }

        if (msg == WM_DESTROY) {
            if (self->window_config.shutdown) {
                PostQuitMessage(0);

                return 0;
            }
        }

        if (self->window_procedure) {
            return self->window_procedure(hwnd, msg, wparam, lparam);
        }
    }

    return DefWindowProcW(hwnd, msg, wparam, lparam);
}

webview::webview(pane::window_config&& window_config,
                 pane::webview_config&& webview_config,
                 std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)>&& window_procedure)
    : window { pane::window(std::move(window_config), std::move(window_procedure)) },
      webview_config { std::move(webview_config) } {
    if (this->options) {
        if (!this->webview_config.environment_options.AdditionalBrowserArguments.empty()) {
            this->options->put_AdditionalBrowserArguments(reinterpret_cast<const wchar_t*>(
                pane::to_utf16(this->webview_config.environment_options.AdditionalBrowserArguments)
                    .data()));
        }

        this->options->put_AllowSingleSignOnUsingOSPrimaryAccount(
            this->webview_config.environment_options.AllowSingleSignOnUsingOSPrimaryAccount);

        if (!this->webview_config.environment_options.Language.empty()) {
            this->options->put_Language(reinterpret_cast<const wchar_t*>(
                pane::to_utf16(this->webview_config.environment_options.Language).data()));
        }

        if (!this->webview_config.environment_options.TargetCompatibleBrowserVersion.empty()) {
            this->options->put_TargetCompatibleBrowserVersion(reinterpret_cast<const wchar_t*>(
                pane::to_utf16(
                    this->webview_config.environment_options.TargetCompatibleBrowserVersion)
                    .data()));
        }
    }

    if (this->options2) {
        this->options2->put_ExclusiveUserDataFolderAccess(
            this->webview_config.environment_options.ExclusiveUserDataFolderAccess);
    }

    if (this->options3) {
        this->options3->put_IsCustomCrashReportingEnabled(
            this->webview_config.environment_options.IsCustomCrashReportingEnabled);
    }

    // if (this->options4) {
    //     this->options4->SetCustomSchemeRegistrations();
    // }

    if (this->options5) {
        this->options5->put_EnableTrackingPrevention(
            this->webview_config.environment_options.EnableTrackingPrevention);
    }

    if (this->options6) {
        this->options6->put_AreBrowserExtensionsEnabled(
            this->webview_config.environment_options.AreBrowserExtensionsEnabled);
    }

    if (this->options7) {
        this->options7->put_ChannelSearchKind(
            this->webview_config.environment_options.ChannelSearchKind);
    }

    if (this->options8) {
        this->options8->put_ScrollBarStyle(this->webview_config.environment_options.ScrollBarStyle);
    }

    CreateCoreWebView2EnvironmentWithOptions(
        this->webview_config.browser_executable_folder.c_str(),
        this->webview_config.user_data_folder.c_str(),
        this->options.get(),
        wil::MakeAgileCallback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [&]([[maybe_unused]] HRESULT error_code,
                ICoreWebView2Environment* created_environment) -> HRESULT {
        if (created_environment) {
            this->environment = wil::com_ptr<ICoreWebView2Environment>(created_environment)
                                    .try_query<ICoreWebView2Environment13>();
        }

        if (this->environment) {
            this->environment->CreateCoreWebView2Controller(
                this->window.window_handle,
                wil::MakeAgileCallback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                    [&]([[maybe_unused]] HRESULT error_code,
                        ICoreWebView2Controller* created_controller) -> HRESULT {
                if (created_controller) {
                    this->controller = wil::com_ptr<ICoreWebView2Controller>(created_controller)
                                           .try_query<ICoreWebView2Controller4>();
                }

                if (this->controller) {
                    this->controller->put_DefaultBackgroundColor(
                        this->window.window_config.background_color.to_webview2_color());

                    RECT client_rect {};
                    GetClientRect(this->window.window_handle, &client_rect);

                    this->controller->put_Bounds(client_rect);

                    wil::com_ptr<ICoreWebView2> created_core;
                    this->controller->get_CoreWebView2(created_core.put());

                    if (created_core) {
                        this->core = created_core.try_query<ICoreWebView2_22>();
                    }

                    if (this->core) {
                        wil::com_ptr<ICoreWebView2Settings> created_settings;
                        this->core->get_Settings(created_settings.put());

                        if (created_settings) {
                            this->settings = created_settings.try_query<ICoreWebView2Settings9>();

                            if (this->settings) {
                                this->settings->put_AreBrowserAcceleratorKeysEnabled(
                                    this->webview_config.settings.AreBrowserAcceleratorKeysEnabled);
                                this->settings->put_AreDefaultContextMenusEnabled(
                                    this->webview_config.settings.AreDefaultContextMenusEnabled);
                                this->settings->put_AreDefaultScriptDialogsEnabled(
                                    this->webview_config.settings.AreDefaultScriptDialogsEnabled);
                                this->settings->put_AreDevToolsEnabled(
                                    this->webview_config.settings.AreDevToolsEnabled);
                                this->settings->put_AreHostObjectsAllowed(
                                    this->webview_config.settings.AreHostObjectsAllowed);
                                this->settings->put_HiddenPdfToolbarItems(
                                    this->webview_config.settings.HiddenPdfToolbarItems);
                                this->settings->put_IsBuiltInErrorPageEnabled(
                                    this->webview_config.settings.IsBuiltInErrorPageEnabled);
                                this->settings->put_IsGeneralAutofillEnabled(
                                    this->webview_config.settings.IsGeneralAutofillEnabled);
                                this->settings->put_IsNonClientRegionSupportEnabled(
                                    this->webview_config.settings.IsNonClientRegionSupportEnabled);
                                this->settings->put_IsPasswordAutosaveEnabled(
                                    this->webview_config.settings.IsPasswordAutosaveEnabled);
                                this->settings->put_IsPinchZoomEnabled(
                                    this->webview_config.settings.IsPinchZoomEnabled);
                                this->settings->put_IsReputationCheckingRequired(
                                    this->webview_config.settings.IsReputationCheckingRequired);
                                this->settings->put_IsScriptEnabled(
                                    this->webview_config.settings.IsScriptEnabled);
                                this->settings->put_IsStatusBarEnabled(
                                    this->webview_config.settings.IsStatusBarEnabled);
                                this->settings->put_IsSwipeNavigationEnabled(
                                    this->webview_config.settings.IsSwipeNavigationEnabled);
                                this->settings->put_IsWebMessageEnabled(
                                    this->webview_config.settings.IsWebMessageEnabled);
                                this->settings->put_IsZoomControlEnabled(
                                    this->webview_config.settings.IsZoomControlEnabled);
                            }
                        }
                    }

                    this->navigate(this->webview_config.home_page);
                }

                return S_OK;
            }).Get());
        }
        return S_OK;
    }).Get());
}

auto webview::navigate(this Self& self, std::u8string_view url) -> void {
    if (self.core) {
        self.core->Navigate(reinterpret_cast<const wchar_t*>(pane::to_utf16(url).data()));
    }
}
} // namespace pane
