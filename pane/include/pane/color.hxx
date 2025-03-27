#pragma once
#include <Windows.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.ViewManagement.h>
#include <WebView2.h>

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
};
} // namespace pane
