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
struct window_message final {
    using Self = window_message;

    auto default_procedure(this const Self& self) -> LRESULT;

    HWND hwnd { nullptr };
    UINT msg { 0 };
    WPARAM wparam { 0 };
    LPARAM lparam { 0 };
};

struct window_config final {
    std::u8string title;
    pane::color background_color;
    bool visible { true };
    HWND parent_hwnd { nullptr };
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
    std::filesystem::path browser_executable_folder;
    std::filesystem::path user_data_folder;
    environment_options environment_options;
    settings settings;
};

template <typename T> struct window_class final {
    using Self = window_class;

    window_class(std::u8string_view class_name, WNDPROC window_procedure)
        : class_name { pane::to_utf16(class_name) },
          wndclass { { sizeof(WNDCLASSEXW) },
                     { 0 },
                     { window_procedure },
                     { 0 },
                     { sizeof(T) },
                     { pane::system::module_handle().value_or(nullptr) },
                     { pane::system::resource_icon().value_or(pane::system::application_icon()) },
                     { pane::system::arrow_cursor() },
                     { nullptr },
                     { nullptr },
                     { reinterpret_cast<const wchar_t*>(this->class_name.data()) },
                     { pane::system::resource_icon().value_or(
                         pane::system::application_icon()) } } {
        if (GetClassInfoExW(this->wndclass.hInstance, this->wndclass.lpszClassName, &this->wndclass)
            == 0) {
            RegisterClassExW(&this->wndclass);
        };
    }
    ~window_class() { UnregisterClassW(this->wndclass.lpszClassName, this->wndclass.hInstance); }

    auto operator()(this const Self& self) -> const WNDCLASSEXW& { return self.wndclass; }

private:
    std::u16string class_name;
    WNDCLASSEXW wndclass;
};

struct window_handle final {
    using Self = window_handle;

    auto destroy(this const Self& self) -> bool;

    auto activate(this const Self& self) -> bool;
    auto show(this const Self& self) -> bool;
    auto hide(this const Self& self) -> bool;

    auto operator()(this const Self& self) -> HWND;
    auto operator()(this Self& self, HWND hwnd) -> void;

private:
    HWND hwnd { nullptr };
};

struct window_background final {
    using Self = window_background;

    window_background(const pane::color& color);
    ~window_background();

    auto operator()(this const Self& self) -> HBRUSH;
    auto operator()(this Self& self, HBRUSH hbrush) -> void;

private:
    HBRUSH hbrush { nullptr };
};

struct window final {
    using Self = window;

    window(pane::window_config&& window_config = {},
           std::function<LRESULT(Self*, pane::window_message)>&& window_procedure
           = [](Self*, pane::window_message window_message) {
                 return window_message.default_procedure();
             });
    ~window();

    window(const Self&) = delete;
    auto operator=(const Self&) -> Self& = delete;

    window(Self&&) noexcept = delete;
    auto operator=(Self&&) noexcept -> Self& = delete;

private:
    static auto window_class_procedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
        -> LRESULT;
    std::function<LRESULT(Self*, pane::window_message)> window_procedure;

public:
    auto create(this Self& self) -> HWND;

    pane::window_config window_config;
    uintptr_t id { pane::random_number<uintptr_t>() };
    pane::window_class<Self> window_class { u8"PaneWindow", window_class_procedure };
    pane::window_background window_background;
    pane::window_handle window_handle;
    RECT client_rect { 0, 0, 0, 0 };
};

struct webview final {
    using Self = webview;

    webview(pane::window_config&& window_config = {},
            pane::webview_config&& webview_config = {},
            std::function<LRESULT(Self*, pane::window_message)>&& window_procedure
            = [](Self*, pane::window_message window_message) {
                  return window_message.default_procedure();
              });
    ~webview();

    webview(const Self&) = delete;
    auto operator=(const Self&) -> Self& = delete;

    webview(Self&&) noexcept = delete;
    auto operator=(Self&&) noexcept -> Self& = delete;

private:
    static auto window_class_procedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
        -> LRESULT;
    std::function<LRESULT(Self*, pane::window_message)> window_procedure;

public:
    auto navigate(this const Self& self, std::u8string_view url) -> void;

    pane::window_config window_config;
    pane::webview_config webview_config;
    uintptr_t id { pane::random_number<uintptr_t>() };
    pane::window_class<Self> window_class { u8"PaneWebView", window_class_procedure };
    pane::window_background window_background;
    pane::window_handle window_handle;
    RECT client_rect { 0, 0, 0, 0 };

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

template <typename T> struct window_manager final {
    using Self = window_manager;

    auto insert(this Self& self, T* window) -> void { self.set.insert(window->window_handle()); }

    auto erase(this Self& self, T* window) -> void {
        self.set.erase(window->window_handle());

        if (self.set.empty()) {
            PostQuitMessage(0);
        }
    }

    auto clear(this Self& self) -> void {
        self.set.clear();

        if (self.set.empty()) {
            PostQuitMessage(0);
        }
    }

    auto size(this const Self& self) -> uint64_t { return self.set.size(); }

    auto contains(this const Self& self, HWND hwnd) -> bool { return self.set.contains(hwnd); }

    auto empty(this const Self& self) -> bool { return self.set.empty(); }

    auto first(this const Self& self) -> HWND { return *self.set.begin(); }

    auto last(this const Self& self) -> HWND { return *self.set.end(); }

private:
    std::set<HWND> set;
};
} // namespace pane
