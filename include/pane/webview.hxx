#pragma once
#include <Windows.h>
#include <wrl.h>
#include <filesystem>
#include <string>
#include <pane/color.hxx>
#include <pane/gdi_plus.hxx>
#include <pane/math.hxx>
#include <pane/system.hxx>
#include <pane/utility.hxx>
#include <pane/window.hxx>
#include <wil/com.h>
#include <wil/wrl.h>
#include <WebView2.h>
#include <WebView2EnvironmentOptions.h>
#include <ada.h>
#include <glaze/glaze.hpp>

namespace pane {
struct virtual_host {
    std::u8string name;
    std::filesystem::path path;
};

struct webview_config final {
    struct environment_options final {
        std::u8string AdditionalBrowserArguments;
        bool AllowSingleSignOnUsingOSPrimaryAccount { false };
        std::u8string Language;
        std::u8string TargetCompatibleBrowserVersion;
        bool ExclusiveUserDataFolderAccess { false };
        bool IsCustomCrashReportingEnabled { false };
        bool EnableTrackingPrevention { true };
        bool AreBrowserExtensionsEnabled { false };
        COREWEBVIEW2_CHANNEL_SEARCH_KIND ChannelSearchKind {
            COREWEBVIEW2_CHANNEL_SEARCH_KIND::COREWEBVIEW2_CHANNEL_SEARCH_KIND_MOST_STABLE
        };
        COREWEBVIEW2_SCROLLBAR_STYLE ScrollBarStyle {
            COREWEBVIEW2_SCROLLBAR_STYLE::COREWEBVIEW2_SCROLLBAR_STYLE_DEFAULT
        };
    };

    struct settings final {
        bool AreBrowserAcceleratorKeysEnabled { true };
        bool AreDefaultContextMenusEnabled { true };
        bool AreDefaultScriptDialogsEnabled { true };
        bool AreDevToolsEnabled { true };
        bool AreHostObjectsAllowed { true };
        COREWEBVIEW2_PDF_TOOLBAR_ITEMS HiddenPdfToolbarItems {
            COREWEBVIEW2_PDF_TOOLBAR_ITEMS::COREWEBVIEW2_PDF_TOOLBAR_ITEMS_NONE
        };
        bool IsBuiltInErrorPageEnabled { true };
        bool IsGeneralAutofillEnabled { true };
        bool IsNonClientRegionSupportEnabled { true };
        bool IsPasswordAutosaveEnabled { true };
        bool IsPinchZoomEnabled { true };
        bool IsReputationCheckingRequired { true };
        bool IsScriptEnabled { true };
        bool IsStatusBarEnabled { true };
        bool IsSwipeNavigationEnabled { true };
        bool IsWebMessageEnabled { true };
        bool IsZoomControlEnabled { true };
    };

    std::u8string home_page { u8"about:blank" };
    std::optional<virtual_host> virtual_host { std::nullopt };
    std::filesystem::path browser_executable_folder;
    std::filesystem::path user_data_folder;
    environment_options environment_options;
    settings settings;
};

struct webview_token {
    using Self = webview_token;

    webview_token() = default;
    ~webview_token() = default;

    webview_token(const Self&) = delete;
    auto operator=(const Self&) -> Self& = delete;

    webview_token(Self&&) noexcept = delete;
    auto operator=(Self&&) noexcept -> Self& = delete;

    auto operator()(this Self& self) -> EventRegistrationToken*;

private:
    EventRegistrationToken token { 0 };
};

enum struct webview_messages : int {
    environment_created = WM_USER,
    controller_created,
    core_created,
    settings_created,
    favicon_changed,
    navigation_completed
};

template <typename T> struct webview {
    using Self = webview;

    struct config {
        window_config window;
        webview_config webview;
    };

    webview(struct window_manager<T>* window_manager = nullptr)
        : window_manager { window_manager } {
        auto config { T::config() };
        this->window_config = config.window;
        this->webview_config = config.webview;

        CreateWindowExW(
            0,
            this->window_class.data.lpszClassName,
            reinterpret_cast<const wchar_t*>(to_utf16(this->window_config.title).data()),
            this->window_config.parent_hwnd ? WS_CHILDWINDOW : WS_OVERLAPPEDWINDOW,
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

        if (this->environment_options) {
            if (!this->webview_config.environment_options.AdditionalBrowserArguments.empty()) {
                this->environment_options->put_AdditionalBrowserArguments(
                    reinterpret_cast<const wchar_t*>(
                        to_utf16(
                            this->webview_config.environment_options.AdditionalBrowserArguments)
                            .data()));
            }

            this->environment_options->put_AllowSingleSignOnUsingOSPrimaryAccount(
                this->webview_config.environment_options.AllowSingleSignOnUsingOSPrimaryAccount);

            if (!this->webview_config.environment_options.Language.empty()) {
                this->environment_options->put_Language(reinterpret_cast<const wchar_t*>(
                    to_utf16(this->webview_config.environment_options.Language).data()));
            }

            if (!this->webview_config.environment_options.TargetCompatibleBrowserVersion.empty()) {
                this->environment_options->put_TargetCompatibleBrowserVersion(
                    reinterpret_cast<const wchar_t*>(
                        to_utf16(
                            this->webview_config.environment_options.TargetCompatibleBrowserVersion)
                            .data()));
            }

            if (auto environment_options2 {
                    this->environment_options.try_query<ICoreWebView2EnvironmentOptions2>() }) {
                environment_options2->put_ExclusiveUserDataFolderAccess(
                    this->webview_config.environment_options.ExclusiveUserDataFolderAccess);
            }

            if (auto environment_options3 {
                    this->environment_options.try_query<ICoreWebView2EnvironmentOptions3>() }) {
                environment_options3->put_IsCustomCrashReportingEnabled(
                    this->webview_config.environment_options.IsCustomCrashReportingEnabled);
            }

            // if (auto environment_options4 {
            //         this->environment_options.try_query<ICoreWebView2EnvironmentOptions4>() }) {
            //     environment_options4->SetCustomSchemeRegistrations();
            // }

            if (auto environment_options5 {
                    this->environment_options.try_query<ICoreWebView2EnvironmentOptions5>() }) {
                environment_options5->put_EnableTrackingPrevention(
                    this->webview_config.environment_options.EnableTrackingPrevention);
            }

            if (auto environment_options6 {
                    this->environment_options.try_query<ICoreWebView2EnvironmentOptions6>() }) {
                environment_options6->put_AreBrowserExtensionsEnabled(
                    this->webview_config.environment_options.AreBrowserExtensionsEnabled);
            }

            if (auto environment_options7 {
                    this->environment_options.try_query<ICoreWebView2EnvironmentOptions7>() }) {
                environment_options7->put_ChannelSearchKind(
                    this->webview_config.environment_options.ChannelSearchKind);
            }

            if (auto environment_options8 {
                    this->environment_options.try_query<ICoreWebView2EnvironmentOptions8>() }) {
                environment_options8->put_ScrollBarStyle(
                    this->webview_config.environment_options.ScrollBarStyle);
            }

            CreateCoreWebView2EnvironmentWithOptions(
                this->webview_config.browser_executable_folder.c_str(),
                this->webview_config.user_data_folder.c_str(),
                this->environment_options.get(),
                Microsoft::WRL::Callback<
                    ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
                    [&](HRESULT /* error_code */,
                        ICoreWebView2Environment* created_environment) -> HRESULT {
                if (created_environment) {
                    this->environment = wil::com_ptr<ICoreWebView2Environment>(created_environment)
                                            .try_query<ICoreWebView2Environment14>();
                }

                if (this->environment) {
                    make_window_message(this->window_handle(),
                                        webview_messages::environment_created,
                                        0,
                                        this->favicon())
                        .send();

                    wil::com_ptr<ICoreWebView2ControllerOptions> controller_options;
                    this->environment->CreateCoreWebView2ControllerOptions(
                        controller_options.put());

                    if (controller_options) {
                        this->controller_options
                            = controller_options.try_query<ICoreWebView2ControllerOptions4>();
                    }

                    if (this->controller_options) {
                        this->controller_options->put_AllowHostInputProcessing(TRUE);
                        this->controller_options->put_DefaultBackgroundColor(
                            COREWEBVIEW2_COLOR { 0, 0, 0, 0 });
                    }

                    this->environment->CreateCoreWebView2ControllerWithOptions(
                        this->window_handle(),
                        this->controller_options.get(),
                        Microsoft::WRL::Callback<
                            ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                            [this](HRESULT /* error_code */,
                                   ICoreWebView2Controller* created_controller) -> HRESULT {
                        if (created_controller) {
                            this->controller
                                = wil::com_ptr<ICoreWebView2Controller>(created_controller)
                                      .try_query<ICoreWebView2Controller4>();
                        }

                        if (this->controller) {
                            make_window_message(this->window_handle(),
                                                webview_messages::controller_created,
                                                0,
                                                this->favicon())
                                .send();

                            RECT client_rect { 0, 0, 0, 0 };
                            GetClientRect(this->window_handle(), &client_rect);
                            this->controller->put_Bounds(client_rect);

                            // https://learn.microsoft.com/en-us/microsoft-edge/webview2/reference/win32/icorewebview2controller?view=webview2-1.0.3351.48#add_acceleratorkeypressed
                            this->controller->add_AcceleratorKeyPressed(
                                Microsoft::WRL::Callback<
                                    ICoreWebView2AcceleratorKeyPressedEventHandler>(
                                    [&](ICoreWebView2Controller* /* sender */,
                                        ICoreWebView2AcceleratorKeyPressedEventArgs* args)
                                        -> HRESULT {
                                UINT key;
                                args->get_VirtualKey(&key);

                                COREWEBVIEW2_PHYSICAL_KEY_STATUS key_status;
                                args->get_PhysicalKeyStatus(&key_status);

                                switch (key) {
                                    case VK_F11: {
                                        if (!key_status.WasKeyDown) {
                                            args->put_Handled(TRUE);
                                            this->window_handle.toggle_fullscreen();
                                        }
                                    } break;
                                    case 'N': {
                                        if (pane::input::is_key_down(VK_LCONTROL)
                                            || pane::input::is_key_down(VK_RCONTROL)) {
                                            if (!key_status.WasKeyDown) {
                                                args->put_Handled(TRUE);
                                                if (this->window_manager) {
                                                    this->window_manager->create();
                                                }
                                            }
                                        }
                                    } break;
                                    case 'W': {
                                        if (pane::input::is_key_down(VK_LCONTROL)
                                            || pane::input::is_key_down(VK_RCONTROL)) {
                                            if (!key_status.WasKeyDown) {
                                                args->put_Handled(TRUE);
                                                SendMessageW(this->window_handle(), WM_CLOSE, 0, 0);
                                            }
                                        }
                                    } break;
                                }

                                return S_OK;
                            }).Get(),
                                this->token.accelerator_key_pressed());

                            wil::com_ptr<ICoreWebView2> created_core;
                            this->controller->get_CoreWebView2(created_core.put());

                            if (created_core) {
                                this->core = created_core.try_query<ICoreWebView2_27>();
                            }

                            if (this->core) {
                                make_window_message(this->window_handle(),
                                                    webview_messages::core_created,
                                                    0,
                                                    this->favicon())
                                    .send();

                                wil::com_ptr<ICoreWebView2Profile> created_profile;
                                this->core->get_Profile(&created_profile);

                                if (created_profile) {
                                    this->profile
                                        = created_profile.try_query<ICoreWebView2Profile8>();
                                }

                                wil::com_ptr<ICoreWebView2Settings> created_settings;
                                this->core->get_Settings(created_settings.put());

                                if (created_settings) {
                                    this->settings
                                        = created_settings.try_query<ICoreWebView2Settings9>();

                                    if (this->settings) {
                                        make_window_message(this->window_handle(),
                                                            webview_messages::settings_created,
                                                            0,
                                                            this->favicon())
                                            .send();

                                        this->settings->put_AreBrowserAcceleratorKeysEnabled(
                                            this->webview_config.settings
                                                .AreBrowserAcceleratorKeysEnabled);
                                        this->settings->put_AreDefaultContextMenusEnabled(
                                            this->webview_config.settings
                                                .AreDefaultContextMenusEnabled);
                                        this->settings->put_AreDefaultScriptDialogsEnabled(
                                            this->webview_config.settings
                                                .AreDefaultScriptDialogsEnabled);
                                        this->settings->put_AreDevToolsEnabled(
                                            this->webview_config.settings.AreDevToolsEnabled);
                                        this->settings->put_AreHostObjectsAllowed(
                                            this->webview_config.settings.AreHostObjectsAllowed);
                                        this->settings->put_HiddenPdfToolbarItems(
                                            this->webview_config.settings.HiddenPdfToolbarItems);
                                        this->settings->put_IsBuiltInErrorPageEnabled(
                                            this->webview_config.settings
                                                .IsBuiltInErrorPageEnabled);
                                        this->settings->put_IsGeneralAutofillEnabled(
                                            this->webview_config.settings.IsGeneralAutofillEnabled);
                                        this->settings->put_IsNonClientRegionSupportEnabled(
                                            this->webview_config.settings
                                                .IsNonClientRegionSupportEnabled);
                                        this->settings->put_IsPasswordAutosaveEnabled(
                                            this->webview_config.settings
                                                .IsPasswordAutosaveEnabled);
                                        this->settings->put_IsPinchZoomEnabled(
                                            this->webview_config.settings.IsPinchZoomEnabled);
                                        this->settings->put_IsReputationCheckingRequired(
                                            this->webview_config.settings
                                                .IsReputationCheckingRequired);
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

                                if (this->webview_config.virtual_host) {
                                    const auto host_name { to_utf16(
                                        (*this->webview_config.virtual_host).name) };

                                    this->core->SetVirtualHostNameToFolderMapping(
                                        reinterpret_cast<const wchar_t*>(host_name.data()),
                                        (*this->webview_config.virtual_host).path.c_str(),
                                        COREWEBVIEW2_HOST_RESOURCE_ACCESS_KIND::
                                            COREWEBVIEW2_HOST_RESOURCE_ACCESS_KIND_ALLOW);
                                }

                                this->core->add_FaviconChanged(
                                    Microsoft::WRL::Callback<
                                        ICoreWebView2FaviconChangedEventHandler>(
                                        [&](ICoreWebView2* /* sender */,
                                            IUnknown* /* args */) -> HRESULT {
                                    this->core->GetFavicon(
                                        COREWEBVIEW2_FAVICON_IMAGE_FORMAT::
                                            COREWEBVIEW2_FAVICON_IMAGE_FORMAT_PNG,
                                        Microsoft::WRL::Callback<
                                            ICoreWebView2GetFaviconCompletedHandler>(
                                            [&](HRESULT /* error_code */,
                                                IStream* icon_stream) -> HRESULT {
                                        this->favicon_status
                                            = Gdiplus::Bitmap { icon_stream }.GetHICON(
                                                &this->favicon());

                                        make_window_message(this->window_handle(),
                                                            webview_messages::favicon_changed,
                                                            0,
                                                            this->favicon())
                                            .send();

                                        return S_OK;
                                    }).Get());

                                    return S_OK;
                                }).Get(),
                                    this->token.favicon_changed());

                                this->core->add_NavigationCompleted(
                                    Microsoft::WRL::Callback<
                                        ICoreWebView2NavigationCompletedEventHandler>(
                                        [&](ICoreWebView2* /* sender */,
                                            ICoreWebView2NavigationCompletedEventArgs* /* args */)
                                            -> HRESULT {
                                    wil::unique_cotaskmem_string title;
                                    this->core->get_DocumentTitle(&title);
                                    this->current_title = to_utf8(title.get());

                                    make_window_message(this->window_handle(),
                                                        webview_messages::navigation_completed,
                                                        0,
                                                        0)
                                        .send();

                                    return S_OK;
                                }).Get(),
                                    this->token.navigation_completed());

                                this->navigate(this->webview_config.home_page);
                            }
                        }

                        return S_OK;
                    }).Get());
                }
                return S_OK;
            }).Get());
        }
    }
    ~webview() {
        if (this->webview_config.virtual_host) {
            this->core->ClearVirtualHostNameToFolderMapping(reinterpret_cast<const wchar_t*>(
                to_utf16((*this->webview_config.virtual_host).name).data()));
        }

        this->controller->remove_AcceleratorKeyPressed(*this->token.accelerator_key_pressed());
        this->core->remove_FaviconChanged(*this->token.favicon_changed());
        this->core->remove_NavigationCompleted(*this->token.navigation_completed());

        this->settings = nullptr;
        this->core = nullptr;
        this->controller = nullptr;
        this->controller_options = nullptr;
        this->environment = nullptr;
        this->environment_options = nullptr;
    }

    webview(const Self&) = delete;
    auto operator=(const Self&) -> Self& = delete;

    webview(Self&&) noexcept = delete;
    auto operator=(Self&&) noexcept -> Self& = delete;

    auto procedure(this T& self, const window_message& window_message) -> LRESULT {
        switch (window_message.msg) {
                // https://learn.microsoft.com/en-us/windows/win32/winmsg/wm-showwindow
            case WM_SHOWWINDOW: {
                if (self.controller) {
                    self.controller->put_IsVisible(window_message.wparam);
                }
            } break;

                // https://learn.microsoft.com/en-us/windows/win32/winmsg/wm-windowposchanged
            case WM_WINDOWPOSCHANGED: {
                if (self.controller) {
                    self.controller->put_Bounds(self.window_handle.position.client_rect);
                }
            } break;
        }

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

    // auto navigate(this const Self& self, std::u8string_view url) -> void {
    //     const auto u16string { to_utf16(url) };

    //     if (self.core) {
    //         self.core->Navigate(reinterpret_cast<const wchar_t*>(u16string.data()));
    //     }
    // }

    auto navigate(this Self& self, const ada::url& url) -> void {
        self.current_url = url;
        const auto u16string { to_utf16(url.get_href()) };

        if (self.core) {
            self.core->Navigate(reinterpret_cast<const wchar_t*>(u16string.data()));
        }
    }

    auto navigate(this Self& self, const std::filesystem::path& path) -> void {
        if (auto parse { ada::parse<ada::url>(path.string()) }; parse) {
            self.current_url = parse.value();
        }

        if (self.core) {
            self.core->Navigate(path.c_str());
        }
    }

    auto navigate_to_string(this Self& self, const std::u8string& string) -> void {
        if (auto parse { ada::parse<ada::url>(reinterpret_cast<const char*>(string.c_str())) };
            parse) {
            self.current_url = parse.value();
        }

        const auto u16string { to_utf16(string) };

        if (self.core) {
            self.core->NavigateToString(reinterpret_cast<const wchar_t*>(u16string.c_str()));
        }
    }

    // auto post_json(this const Self& self, std::u8string_view message) -> void {
    //     auto conv { to_utf16(message) };

    //     if (self.core) {
    //         self.core->PostWebMessageAsJson(reinterpret_cast<wchar_t*>(conv.data()));
    //     }
    // }

    template <typename U> auto post_json(this const Self& self, U& value) -> void {
        std::u8string buffer;

        [[maybe_unused]] auto ec { glz::write_json(value, buffer) };

        auto conv { to_utf16(buffer) };

        if (self.core) {
            self.core->PostWebMessageAsJson(reinterpret_cast<wchar_t*>(conv.data()));
        }

        // self.post_json(buffer);
    }

    auto post_string(this const Self& self, std::u8string_view message) -> void {
        auto conv { to_utf16(message) };

        if (self.core) {
            self.core->PostWebMessageAsString(reinterpret_cast<wchar_t*>(conv.data()));
        }
    }

    auto devtools(this const Self& self) -> void {
        if (self.core) {
            self.core->OpenDevToolsWindow();
        }
    }

    auto clear_browsing_data(this Self& self) -> void {
        if (self.profile) {
            self.profile->ClearBrowsingDataAll(
                Microsoft::WRL::Callback<ICoreWebView2ClearBrowsingDataCompletedHandler>(
                    [](HRESULT /* error_code */) -> HRESULT {
                return S_OK;
            }).Get());
        }
    }

    window_class<T> window_class { u8"pane_webview" };
    window_config window_config;
    webview_config webview_config;
    window_background window_background { system::dark_mode() ? window_config.bg_dark
                                                              : window_config.bg_light };
    window_handle window_handle;
    window_manager<T>* window_manager { nullptr };

    struct event_token {
        webview_token accelerator_key_pressed;
        webview_token favicon_changed;
        webview_token navigation_completed;
    };
    event_token token;
    gdi_plus gdi_plus;
    window_icon favicon;
    Gdiplus::Status favicon_status;
    ada::url current_url;
    std::u8string current_title;
    wil::com_ptr<ICoreWebView2Settings9> settings;
    wil::com_ptr<ICoreWebView2EnvironmentOptions> environment_options {
        Microsoft::WRL::Make<CoreWebView2EnvironmentOptions>()
    };
    wil::com_ptr<ICoreWebView2Environment14> environment;
    wil::com_ptr<ICoreWebView2ControllerOptions4> controller_options;
    wil::com_ptr<ICoreWebView2Controller4> controller;
    wil::com_ptr<ICoreWebView2_27> core;
    wil::com_ptr<ICoreWebView2Profile8> profile;
};
} // namespace pane
