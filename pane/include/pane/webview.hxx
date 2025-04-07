#pragma once
#include <Windows.h>
#include <filesystem>
#include <functional>
#include <pane/system.hxx>
#include <wil/com.h>
#include <wil/resource.h>
#include <wil/wrl.h>
#include <WebView2.h>
#include <WebView2EnvironmentOptions.h>

namespace pane {
struct webview final {
    using Self = webview;
    struct WebViewEnvironmentOptions {
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

        wil::com_ptr<ICoreWebView2EnvironmentOptions> options {
            Microsoft::WRL::Make<CoreWebView2EnvironmentOptions>()
        };
        wil::com_ptr<ICoreWebView2EnvironmentOptions2> options2 {
            options.try_query<ICoreWebView2EnvironmentOptions2>()
        };
        wil::com_ptr<ICoreWebView2EnvironmentOptions3> options3 {
            options.try_query<ICoreWebView2EnvironmentOptions3>()
        };
        wil::com_ptr<ICoreWebView2EnvironmentOptions4> options4 {
            options.try_query<ICoreWebView2EnvironmentOptions4>()
        };
        wil::com_ptr<ICoreWebView2EnvironmentOptions5> options5 {
            options.try_query<ICoreWebView2EnvironmentOptions5>()
        };
        wil::com_ptr<ICoreWebView2EnvironmentOptions6> options6 {
            options.try_query<ICoreWebView2EnvironmentOptions6>()
        };
        wil::com_ptr<ICoreWebView2EnvironmentOptions7> options7 {
            options.try_query<ICoreWebView2EnvironmentOptions7>()
        };
        wil::com_ptr<ICoreWebView2EnvironmentOptions8> options8 {
            options.try_query<ICoreWebView2EnvironmentOptions8>()
        };
    };

    struct WebViewSettings {
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

        wil::com_ptr<ICoreWebView2Settings9> settings9;
    };

    webview(HWND parent_hwnd);
    ~webview() = default;

    WebViewEnvironmentOptions environment_options;
    WebViewSettings settings;
    std::u8string home_page { u8"about:blank" };
    std::filesystem::path browser_executable_folder;
    std::filesystem::path user_data_folder;
    wil::com_ptr<ICoreWebView2Environment13> environment13;
    wil::com_ptr<ICoreWebView2Controller4> controller4;
    wil::com_ptr<ICoreWebView2_22> core22;
};
} // namespace pane
