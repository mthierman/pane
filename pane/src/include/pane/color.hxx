#pragma once
#include <Windows.h>
#include <objbase.h>
#include <format>
#include <limits>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.ViewManagement.h>
#include <WebView2.h>
#include <pane/system.hxx>

namespace pane {
struct color final {
    using Self = color;

    color() = default;
    ~color() = default;

    color(const Self&) = default;
    auto operator=(const Self&) -> Self& = default;

    color(Self&&) noexcept = default;
    auto operator=(Self&&) noexcept -> Self& = default;

    color(uint8_t r, uint8_t g, uint8_t b, uint8_t alpha = std::numeric_limits<uint8_t>::max());

    explicit color(const COLORREF& colorref, uint8_t alpha = std::numeric_limits<uint8_t>::max());

    explicit color(const winrt::Windows::UI::Color& winrt_color);
    explicit color(const winrt::Windows::UI::Color& winrt_color, uint8_t alpha);

    explicit color(winrt::Windows::UI::ViewManagement::UIColorType winrt_ui_color_type);
    explicit color(winrt::Windows::UI::ViewManagement::UIColorType winrt_ui_color_type,
                   uint8_t alpha);

    explicit color(const COREWEBVIEW2_COLOR& webview2_color);
    explicit color(const COREWEBVIEW2_COLOR& webview2_color, uint8_t alpha);

    auto to_hex(this const Self& self) -> std::u8string;
    auto to_colorref(this const Self& self) -> COLORREF;
    auto to_hbrush(this const Self& self) -> HBRUSH;
    auto to_winrt_color(this const Self& self) -> winrt::Windows::UI::Color;
    auto to_webview2_color(this const Self& self) -> COREWEBVIEW2_COLOR;

    auto is_dark(this const Self& self) -> bool;

    uint8_t r { std::numeric_limits<uint8_t>::min() };
    uint8_t g { std::numeric_limits<uint8_t>::min() };
    uint8_t b { std::numeric_limits<uint8_t>::min() };
    uint8_t a { std::numeric_limits<uint8_t>::min() };
    winrt::Windows::UI::ViewManagement::UISettings ui_settings { pane::system::ui_settings() };
};

struct system_colors {
    pane::color accent { pane::color { winrt::Windows::UI::ViewManagement::UIColorType::Accent } };
    pane::color accent_dark_1 { pane::color {
        winrt::Windows::UI::ViewManagement::UIColorType::AccentDark1 } };
    pane::color accent_dark_2 { pane::color {
        winrt::Windows::UI::ViewManagement::UIColorType::AccentDark2 } };
    pane::color accent_dark_3 { pane::color {
        winrt::Windows::UI::ViewManagement::UIColorType::AccentDark3 } };
    pane::color accent_light_1 { pane::color {
        winrt::Windows::UI::ViewManagement::UIColorType::AccentLight1 } };
    pane::color accent_light_2 { pane::color {
        winrt::Windows::UI::ViewManagement::UIColorType::AccentLight2 } };
    pane::color accent_light_3 { pane::color {
        winrt::Windows::UI::ViewManagement::UIColorType::AccentLight3 } };
    pane::color background { pane::color {
        winrt::Windows::UI::ViewManagement::UIColorType::Background } };
    pane::color foreground { pane::color {
        winrt::Windows::UI::ViewManagement::UIColorType::Foreground } };
};
} // namespace pane

namespace std {
template <> struct formatter<pane::color> : formatter<string_view> {
    auto format(const pane::color& color, format_context& context) const noexcept {
        return formatter<string_view>::format(
            std::format("#{:0>2x}{:0>2x}{:0>2x}{:0>2x}", color.r, color.g, color.b, color.a),
            context);
    }
};

template <> struct formatter<pane::color, wchar_t> : formatter<wstring_view, wchar_t> {
    auto format(const pane::color& color, wformat_context& context) const noexcept {
        return formatter<wstring_view, wchar_t>::format(
            std::format(L"#{:0>2x}{:0>2x}{:0>2x}{:0>2x}", color.r, color.g, color.b, color.a),
            context);
    }
};
} // namespace std
