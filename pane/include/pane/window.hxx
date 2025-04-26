#pragma once
#include <Windows.h>
#include <filesystem>
#include <functional>
#include <set>
#include <pane/color.hxx>
#include <pane/math.hxx>
#include <pane/system.hxx>
#include <wil/com.h>
#include <wil/resource.h>
#include <wil/wrl.h>
#include <WebView2.h>
#include <WebView2EnvironmentOptions.h>

namespace pane {
struct window_config final {
    std::u8string title;
    pane::color background_color;
    bool visible { true };
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

struct window_manager {
    using Self = window_manager;

    auto insert(this Self& self, HWND hwnd) -> void;
    auto erase(this Self& self, HWND hwnd) -> void;
    auto clear(this Self& self) -> void;

    auto size(this const Self& self) -> uint64_t;

    auto contains(this const Self& self, HWND hwnd) -> bool;
    auto empty(this const Self& self) -> bool;

    auto first(this const Self& self) -> HWND;
    auto last(this const Self& self) -> HWND;

    std::set<HWND> set;
};

struct window_message {
    HWND hwnd;
    UINT msg;
    WPARAM wparam;
    LPARAM lparam;
};

struct window final {
    using Self = window;

    struct message {
        Self* window;
        window_message window_message;
    };

    window(pane::window_config&& window_config = {},
           std::function<LRESULT(pane::window_message)>&& procedure
           = [](pane::window_message message) {
                 return DefWindowProcW(message.hwnd, message.msg, message.wparam, message.lparam);
             });
    ~window();

    window(const Self&) = delete;
    auto operator=(const Self&) -> Self& = delete;

    window(Self&&) noexcept = delete;
    auto operator=(Self&&) noexcept -> Self& = delete;

    auto create(this Self& self, const WNDCLASSEXW& window_class, std::u8string_view window_name)
        -> void;
    auto show(this const Self& self) -> bool;
    auto hide(this const Self& self) -> bool;
    static auto default_procedure(pane::window_message message) -> LRESULT;

    HWND window_handle;
    HBRUSH window_background;
    RECT client_rect;
    pane::window_config window_config;

private:
    static auto class_procedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> LRESULT;
    uint64_t window_id { pane::random_number<uint64_t>() };
    std::wstring class_window_name { L"PaneWindow" + std::to_wstring(window_id) };

    WNDCLASSEXW window_class {
        .cbSize { sizeof(WNDCLASSEXW) },
        .style { 0 },
        .lpfnWndProc { class_procedure },
        .cbClsExtra { 0 },
        .cbWndExtra { sizeof(Self) },
        .hInstance { pane::system::module_handle().value_or(nullptr) },
        .hIcon { pane::system::resource_icon().value_or(pane::system::application_icon()) },
        .hCursor { pane::system::arrow_cursor() },
        .hbrBackground { nullptr },
        .lpszMenuName { nullptr },
        .lpszClassName { class_window_name.data() },
        .hIconSm { pane::system::resource_icon().value_or(pane::system::application_icon()) }
    };
    std::function<LRESULT(pane::window_message)> window_procedure;
};

struct webview final {
    using Self = webview;

    struct message {
        Self* webview;
        window_message window_message;
    };

    webview(pane::window_config&& window_config = {},
            pane::webview_config&& webview_config = {},
            std::function<LRESULT(pane::window_message)>&& procedure
            = [](pane::window_message message) {
                  return DefWindowProcW(message.hwnd, message.msg, message.wparam, message.lparam);
              });
    ~webview() = default;

    webview(const Self&) = delete;
    auto operator=(const Self&) -> Self& = delete;

    webview(Self&&) noexcept = delete;
    auto operator=(Self&&) noexcept -> Self& = delete;

    auto navigate(this const Self& self, std::u8string_view url) -> void;

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

    std::function<LRESULT(pane::window_message)> window_procedure;
    std::function<LRESULT(pane::window_message)> webview_procedure;
    pane::window window;
};
} // namespace pane
