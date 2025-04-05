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

    color(const Self& color) = default;
    auto operator=(const Self& color) -> Self& = default;
    color(Self&& color) noexcept = default;
    auto operator=(Self&& color) noexcept -> Self& = default;

    explicit color(uint8_t r,
                   uint8_t g,
                   uint8_t b,
                   uint8_t alpha = std::numeric_limits<uint8_t>::max());

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

    uint8_t r { std::numeric_limits<uint8_t>::max() };
    uint8_t g { std::numeric_limits<uint8_t>::max() };
    uint8_t b { std::numeric_limits<uint8_t>::max() };
    uint8_t a { std::numeric_limits<uint8_t>::max() };
    winrt::Windows::UI::ViewManagement::UISettings ui_settings { pane::system::ui_settings() };
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
