#pragma once
#include <Windows.h>
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
    struct EnvironmentOptions {
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

    webview();
    ~webview() = default;

    EnvironmentOptions environment_options;
};
} // namespace pane
