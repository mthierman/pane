#include <pane/color.hxx>

namespace pane {
color::color(uint8_t r, uint8_t g, uint8_t b, uint8_t alpha)
    : r { r },
      g { g },
      b { b },
      a { alpha } { }

color::color(const COLORREF& colorref, uint8_t alpha)
    : r { GetRValue(colorref) },
      g { GetGValue(colorref) },
      b { GetBValue(colorref) },
      a { alpha } { }

color::color(const winrt::Windows::UI::Color& winrt_color)
    : r { winrt_color.R },
      g { winrt_color.G },
      b { winrt_color.B },
      a { winrt_color.A } { }

color::color(const winrt::Windows::UI::Color& winrt_color, uint8_t alpha)
    : r { winrt_color.R },
      g { winrt_color.G },
      b { winrt_color.B },
      a { alpha } { }

color::color(winrt::Windows::UI::ViewManagement::UIColorType winrt_ui_color_type) {
    auto color { ui_settings.GetColorValue(winrt_ui_color_type) };

    r = color.R;
    g = color.G;
    b = color.B;
    a = color.A;
}

color::color(winrt::Windows::UI::ViewManagement::UIColorType winrt_ui_color_type, uint8_t alpha) {
    auto color { ui_settings.GetColorValue(winrt_ui_color_type) };

    r = color.R;
    g = color.G;
    b = color.B;
    a = alpha;
}

color::color(const COREWEBVIEW2_COLOR& webview2_color)
    : r { webview2_color.R },
      g { webview2_color.G },
      b { webview2_color.B },
      a { webview2_color.A } { }

color::color(const COREWEBVIEW2_COLOR& webview2_color, uint8_t alpha)
    : r { webview2_color.R },
      g { webview2_color.G },
      b { webview2_color.B },
      a { alpha } { }

auto color::to_hex(this const Self& self) -> std::u8string {
    auto formatted { std::format("#{:0>2x}{:0>2x}{:0>2x}{:0>2x}", self.r, self.g, self.b, self.a) };

    return { formatted.begin(), formatted.end() };
}

auto color::to_colorref(this const Self& self) -> COLORREF { return RGB(self.r, self.g, self.b); }

auto color::to_hbrush(this const Self& self) -> HBRUSH {
    return CreateSolidBrush(self.to_colorref());
}

auto color::to_winrt_color(this const Self& self) -> winrt::Windows::UI::Color {
    return winrt::Windows::UI::Color { .A { self.a }, .R { self.r }, .G { self.g }, .B { self.b } };
}

auto color::to_webview2_color(this const Self& self) -> COREWEBVIEW2_COLOR {
    return COREWEBVIEW2_COLOR { .A { self.a }, .R { self.r }, .G { self.g }, .B { self.b } };
}

auto color::is_dark(this const Self& self) -> bool {
    return (((5 * self.g) + (2 * self.r) + self.b) < (8 * 128)) ? true : false;
}
} // namespace pane
