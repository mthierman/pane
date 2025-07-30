#pragma once
#include <Windows.h>
#include <wrl.h>
#include <filesystem>
#include <functional>
#include <utility>
#include <pane/color.hxx>
#include <pane/gdi_plus.hxx>
#include <pane/math.hxx>
#include <pane/system.hxx>
#include <pane/window.hxx>
#include <wil/com.h>
#include <WebView2.h>
#include <WebView2EnvironmentOptions.h>
#include <ada.h>

namespace pane {
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
    std::optional<std::pair<std::u8string, std::filesystem::path>> virtual_host_name_map {
        std::nullopt
    };
    std::function<HRESULT()> creation_callback;
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

struct webview final {
    using Self = webview;

    friend struct window_class<Self>;

    webview(pane::window_config&& window_config = {},
            pane::webview_config&& webview_config = {},
            std::function<LRESULT(pane::window_message)>&& window_procedure
            = [](pane::window_message window_message) -> LRESULT {
                return window_message.default_procedure();
            });
    ~webview();

    webview(const Self&) = delete;
    auto operator=(const Self&) -> Self& = delete;

    webview(Self&&) noexcept = delete;
    auto operator=(Self&&) noexcept -> Self& = delete;

    auto default_procedure(this Self& self, const pane::window_message& window_message) -> LRESULT;

private:
    std::function<LRESULT(pane::window_message)> custom_procedure;

public:
    // auto navigate(this const Self& self, std::u8string_view url) -> void;
    auto navigate(this Self& self, const ada::url& url) -> void;
    auto navigate(this Self& self, const std::filesystem::path& path) -> void;
    auto navigate_to_string(this Self& self, const std::u8string& string) -> void;

    struct event_token {
        pane::webview_token accelerator_key_pressed;
        pane::webview_token favicon_changed;
        pane::webview_token source_changed;
    };
    event_token token;

    pane::gdi_plus gdi_plus;
    pane::window_icon favicon;
    Gdiplus::Status favicon_status;
    ada::url current_url;
    std::u8string current_title;

    pane::window_config window_config;
    pane::webview_config webview_config;
    pane::window_class<Self> window_class { u8"PaneWebView" };
    pane::window_background window_background { pane::system::dark_mode()
                                                    ? window_config.dark_background
                                                    : window_config.light_background };
    pane::window_handle window_handle;

    wil::com_ptr<ICoreWebView2Settings9> settings;
    wil::com_ptr<ICoreWebView2EnvironmentOptions> environment_options {
        Microsoft::WRL::Make<CoreWebView2EnvironmentOptions>()
    };
    wil::com_ptr<ICoreWebView2Environment14> environment;
    wil::com_ptr<ICoreWebView2ControllerOptions4> controller_options;
    wil::com_ptr<ICoreWebView2Controller4> controller;
    wil::com_ptr<ICoreWebView2_27> core;
};
} // namespace pane
