#pragma once
#include <Windows.h>
#include <filesystem>
#include <functional>
#include <pane/system.hxx>
#include <pane/color.hxx>
#include <wil/com.h>
#include <wil/resource.h>
#include <wil/wrl.h>
#include <WebView2.h>
#include <WebView2EnvironmentOptions.h>

namespace pane {
struct webview final {
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

    environment_options environment_options;
    settings settings;
    std::filesystem::path browser_executable_folder;
    std::filesystem::path user_data_folder;
    std::u8string home_page { u8"about:blank" };

    wil::com_ptr<ICoreWebView2Settings9> core_settings;
    wil::com_ptr<ICoreWebView2Environment13> core_environment;
    wil::com_ptr<ICoreWebView2Controller4> core_controller;
    wil::com_ptr<ICoreWebView2_22> core_core;
    wil::com_ptr<ICoreWebView2EnvironmentOptions> core_options {
        Microsoft::WRL::Make<CoreWebView2EnvironmentOptions>()
    };
    wil::com_ptr<ICoreWebView2EnvironmentOptions2> core_options2 {
        core_options.try_query<ICoreWebView2EnvironmentOptions2>()
    };
    wil::com_ptr<ICoreWebView2EnvironmentOptions3> core_options3 {
        core_options.try_query<ICoreWebView2EnvironmentOptions3>()
    };
    wil::com_ptr<ICoreWebView2EnvironmentOptions4> core_options4 {
        core_options.try_query<ICoreWebView2EnvironmentOptions4>()
    };
    wil::com_ptr<ICoreWebView2EnvironmentOptions5> core_options5 {
        core_options.try_query<ICoreWebView2EnvironmentOptions5>()
    };
    wil::com_ptr<ICoreWebView2EnvironmentOptions6> core_options6 {
        core_options.try_query<ICoreWebView2EnvironmentOptions6>()
    };
    wil::com_ptr<ICoreWebView2EnvironmentOptions7> core_options7 {
        core_options.try_query<ICoreWebView2EnvironmentOptions7>()
    };
    wil::com_ptr<ICoreWebView2EnvironmentOptions8> core_options8 {
        core_options.try_query<ICoreWebView2EnvironmentOptions8>()
    };
};

struct window final {
    using Self = window;

    struct config final {
        std::u8string title;
        pane::color background_color;
        bool visible { false };
        bool webview { false };
    };

    window(const pane::window::config& window_config = pane::window::config {},
           std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)>&& window_procedure
           = [](HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
                 return DefWindowProcW(hwnd, msg, wparam, lparam);
             });
    ~window();

    auto client_rect(this const Self& self) -> RECT;
    static auto get_instance(HWND hwnd) -> Self*;

    auto create_webview(this Self& self, const pane::window::config& window_config) -> void;
    auto navigate(this Self& self, std::u8string_view url) -> void;

private:
    static auto class_window_procedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
        -> LRESULT;

    HWND window_handle;
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
    webview webview;
};
} // namespace pane
