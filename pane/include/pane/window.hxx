#pragma once
#include <Windows.h>
#include <filesystem>
#include <optional>
#include <functional>
#include <pane/system.hxx>
#include <pane/color.hxx>
#include <wil/com.h>
#include <wil/resource.h>
#include <wil/wrl.h>
#include <WebView2.h>
#include <WebView2EnvironmentOptions.h>

namespace pane {
struct window_config final {
    std::u8string title;
    pane::color background_color;
    bool visible { false };
    bool shutdown { false };
};

struct webview_config {
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
    std::filesystem::path browser_executable_folder;
    std::filesystem::path user_data_folder;
    environment_options environment_options;
    settings settings;
};

struct window final {
    using Self = window;

    window(pane::window_config&& window_config = {},
           std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)>&& procedure
           = [](HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
                 return DefWindowProcW(hwnd, msg, wparam, lparam);
             });
    ~window();

    auto activate(this const Self& self) -> bool;
    auto show(this const Self& self) -> bool;
    auto hide(this const Self& self) -> bool;

    auto client_rect(this const Self& self) -> RECT;
    static auto get_instance(HWND hwnd) -> Self*;

    HWND window_handle;
    pane::window_config window_config;

private:
    static auto class_window_procedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
        -> LRESULT;

    WNDCLASSEXW window_class {
        .cbSize { sizeof(WNDCLASSEXW) },
        .style { 0 },
        .lpfnWndProc { class_window_procedure },
        .cbClsExtra { 0 },
        .cbWndExtra { sizeof(Self) },
        .hInstance { pane::system::module_handle().value_or(nullptr) },
        .hIcon { pane::system::resource_icon().value_or(pane::system::application_icon()) },
        .hCursor { pane::system::arrow_cursor() },
        .hbrBackground { nullptr },
        .lpszMenuName { nullptr },
        .lpszClassName { L"PaneWindow" },
        .hIconSm { pane::system::resource_icon().value_or(pane::system::application_icon()) }
    };
    std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)> window_procedure;
};

struct webview final {
    using Self = webview;

    webview(pane::window_config&& window_config = {},
            pane::webview_config&& webview_config = {},
            std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)>&& procedure
            = [](HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
                  return DefWindowProcW(hwnd, msg, wparam, lparam);
              });
    ~webview() = default;

    auto navigate(this Self& self, std::u8string_view url) -> void;

    pane::window window;
    pane::webview_config webview_config;
    wil::com_ptr<ICoreWebView2Settings9> settings;
    wil::com_ptr<ICoreWebView2Environment13> environment;
    wil::com_ptr<ICoreWebView2Controller4> controller;
    wil::com_ptr<ICoreWebView2_22> core;
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
} // namespace pane
