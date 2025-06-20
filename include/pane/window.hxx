#pragma once
#include <Windows.h>
#include <wrl.h>
#include <filesystem>
#include <functional>
#include <set>
#include <utility>
#include <pane/color.hxx>
#include <pane/math.hxx>
#include <pane/system.hxx>
#include <wil/com.h>
#include <WebView2.h>
#include <WebView2EnvironmentOptions.h>
#include <ada.h>

namespace pane {
struct window_position final {
    using Self = window_position;

    bool fullscreen { false };
    bool maximized { false };
    bool minimized { false };
    RECT client_rect { 0, 0, 0, 0 };
    WINDOWPLACEMENT window_placement { .length { sizeof(WINDOWPLACEMENT) } };
};

enum struct window_backdrop { automatic, mica, mica_alt, acrylic, none };

struct window_message final {
    using Self = window_message;

    auto default_procedure(this const Self& self) -> LRESULT;

    HWND hwnd { nullptr };
    UINT event { 0 };
    WPARAM wparam { 0 };
    LPARAM lparam { 0 };
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

    window_class(const Self&) = delete;
    auto operator=(const Self&) -> Self& = delete;

    window_class(Self&&) noexcept = delete;
    auto operator=(Self&&) noexcept -> Self& = delete;

    auto operator()(this const Self& self) -> const WNDCLASSEXW& { return self.wndclass; }

private:
    std::u16string class_name;
    WNDCLASSEXW wndclass;
};

struct window_handle final {
    using Self = window_handle;

    window_handle() = default;
    ~window_handle();

    window_handle(const Self&) = delete;
    auto operator=(const Self&) -> Self& = delete;

    window_handle(Self&&) noexcept = delete;
    auto operator=(Self&&) noexcept -> Self& = delete;

    auto activate(this const Self& self) -> bool;
    auto show(this const Self& self) -> bool;
    auto hide(this const Self& self) -> bool;

    auto maximize(this const Self& self) -> bool;
    auto minimize(this const Self& self) -> bool;
    auto restore(this const Self& self) -> bool;
    auto toggle_fullscreen(this Self& self) -> bool;

    auto immersive_dark_mode(this const Self& self, bool enable) -> HRESULT;
    auto cloak(this const Self& self, bool enable) -> HRESULT;
    auto backdrop(this const Self& self, pane::window_backdrop window_backdrop) -> HRESULT;
    auto border_color(this const Self& self, const pane::color& color) -> HRESULT;
    auto caption_color(this const Self& self, const pane::color& color) -> HRESULT;
    auto text_color(this const Self& self, const pane::color& color) -> HRESULT;

    auto operator()(this const Self& self) -> HWND;
    auto operator()(this Self& self, HWND hwnd) -> void;

    pane::window_position window_position;

private:
    HWND hwnd { nullptr };
};

struct window_background final {
    using Self = window_background;

    explicit window_background(const pane::color& color);
    ~window_background();

    window_background(const Self&) = delete;
    auto operator=(const Self&) -> Self& = delete;

    window_background(Self&&) noexcept = delete;
    auto operator=(Self&&) noexcept -> Self& = delete;

    auto operator()(this const Self& self) -> HBRUSH;
    auto operator()(this Self& self, const pane::color& color) -> void;

private:
    HBRUSH hbrush { nullptr };
};

struct window_icon final {
    using Self = window_icon;

    window_icon() = default;
    explicit window_icon(HICON hicon);
    ~window_icon();

    window_icon(const Self&) = delete;
    auto operator=(const Self&) -> Self& = delete;

    window_icon(Self&&) noexcept = delete;
    auto operator=(Self&&) noexcept -> Self& = delete;

    auto operator()(this Self& self) -> HICON&;
    auto operator()(this Self& self, HICON hicon) -> void;

private:
    HICON hicon { nullptr };
};

struct window;

struct window_config final {
    std::u8string title;
    pane::color dark_background;
    pane::color light_background;
    bool visible { true };
    HWND parent_hwnd { nullptr };
};

struct window final {
    using Self = window;

    window(pane::window_config&& window_config = {},
           std::function<LRESULT(Self*, pane::window_message)>&& window_procedure
           = [](Self*, pane::window_message window_message) {
                 return window_message.default_procedure();
             });
    ~window() = default;

    window(const Self&) = delete;
    auto operator=(const Self&) -> Self& = delete;

    window(Self&&) noexcept = delete;
    auto operator=(Self&&) noexcept -> Self& = delete;

    auto default_procedure(this Self& self, const pane::window_message& window_message) -> LRESULT;

private:
    static auto window_class_procedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
        -> LRESULT;
    std::function<LRESULT(Self*, pane::window_message)> window_procedure;

    auto set_theme(this Self& self) -> void;

public:
    auto create(this Self& self) -> HWND;

    pane::window_config window_config;
    uintptr_t id { pane::random_number<uintptr_t>() };
    pane::window_class<Self> window_class { u8"PaneWindow", window_class_procedure };
    pane::window_background window_background { pane::system::dark_mode()
                                                    ? window_config.dark_background
                                                    : window_config.light_background };
    pane::window_handle window_handle;
    UINT dpi { GetDpiForWindow(window_handle()) };
    float scale_factor { static_cast<float>(dpi) / static_cast<float>(USER_DEFAULT_SCREEN_DPI) };
};

struct webview;

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
    std::function<void(pane::webview*)> creation_callback;
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

    auto default_procedure(this Self& self, const pane::window_message& window_message) -> LRESULT;

private:
    static auto window_class_procedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
        -> LRESULT;
    std::function<LRESULT(Self*, pane::window_message)> window_procedure;

    auto set_theme(this Self& self) -> void;

public:
    auto create(this Self& self) -> HWND;

    // auto navigate(this const Self& self, std::u8string_view url) -> void;
    auto navigate(this const Self& self, const ada::url& url) -> void;
    auto navigate(this const Self& self, const std::filesystem::path& path) -> void;
    auto navigate_to_string(this const Self& self, const std::u8string& string) -> void;

    pane::window_config window_config;
    pane::webview_config webview_config;
    uintptr_t id { pane::random_number<uintptr_t>() };
    pane::window_class<Self> window_class { u8"PaneWebView", window_class_procedure };
    pane::window_background window_background { pane::system::dark_mode()
                                                    ? window_config.dark_background
                                                    : window_config.light_background };
    pane::window_handle window_handle;
    pane::window_position window_position;
    UINT dpi { GetDpiForWindow(window_handle()) };
    float scale_factor { static_cast<float>(dpi) / static_cast<float>(USER_DEFAULT_SCREEN_DPI) };

    wil::com_ptr<ICoreWebView2Settings9> settings;
    wil::com_ptr<ICoreWebView2Environment14> environment;
    wil::com_ptr<ICoreWebView2Controller4> controller;
    wil::com_ptr<ICoreWebView2_27> core;
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

struct window_manager final {
    using Self = window_manager;

    auto insert(this Self& self, const pane::window_handle& window_handle) -> void;
    auto erase(this Self& self, const pane::window_handle& window_handle) -> void;
    auto clear(this Self& self) -> void;
    auto size(this const Self& self) -> uint64_t;
    auto contains(this const Self& self, HWND hwnd) -> bool;
    auto empty(this const Self& self) -> bool;
    auto first(this const Self& self) -> HWND;
    auto last(this const Self& self) -> HWND;

private:
    std::set<HWND> set;
};
} // namespace pane
