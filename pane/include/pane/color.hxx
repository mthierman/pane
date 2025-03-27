#pragma once
#include <Windows.h>
#include <combaseapi.h>
#include <limits>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.ViewManagement.h>
#include <WebView2.h>
#include <pane/string.hxx>

namespace winrt {
using namespace winrt::Windows::UI;
using namespace winrt::Windows::UI::ViewManagement;
}; // namespace winrt

namespace pane {
struct color {
    using Self = color;

    color() = default;
    ~color() = default;

    color(Self&& color) noexcept = default;
    auto operator=(Self&& color) noexcept -> Self& = default;

    color(const Self& color) = default;
    auto operator=(const Self& color) -> Self& = default;

    explicit color(uint8_t r,
                   uint8_t g,
                   uint8_t b,
                   uint8_t a = std::numeric_limits<uint8_t>::max());

    auto to_hex(this Self& self) -> string;
    auto to_colorref(this Self& self) -> COLORREF;
    auto to_hbrush(this Self& self) -> HBRUSH;

    uint8_t r { std::numeric_limits<uint8_t>::max() };
    uint8_t g { std::numeric_limits<uint8_t>::max() };
    uint8_t b { std::numeric_limits<uint8_t>::max() };
    uint8_t a { std::numeric_limits<uint8_t>::max() };
};
} // namespace pane
