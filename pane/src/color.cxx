#include <pane/color.hxx>
#include <format>

namespace pane {
color::color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
    : r { r },
      g { g },
      b { b },
      a { a } { }

color::color(const COLORREF& colorref, uint8_t a)
    : r { GetRValue(colorref) },
      g { GetGValue(colorref) },
      b { GetBValue(colorref) },
      a { a } { }

color::color(const winrt::Color& winrt_color)
    : r { winrt_color.R },
      g { winrt_color.G },
      b { winrt_color.B },
      a { winrt_color.A } { }

color::color(const winrt::Color& winrt_color, uint8_t a)
    : r { winrt_color.R },
      g { winrt_color.G },
      b { winrt_color.B },
      a { a } { }

color::color(const COREWEBVIEW2_COLOR& webview2_color)
    : r { webview2_color.R },
      g { webview2_color.G },
      b { webview2_color.B },
      a { webview2_color.A } { }

color::color(const COREWEBVIEW2_COLOR& webview2_color, uint8_t a)
    : r { webview2_color.R },
      g { webview2_color.G },
      b { webview2_color.B },
      a { a } { }

auto color::to_hex(this Self& self) -> std::u8string {
    auto formatted { std::format("{}", self) };

    return { formatted.begin(), formatted.end() };
}

auto color::to_colorref(this Self& self) -> COLORREF { return RGB(self.r, self.g, self.b); }

auto color::to_hbrush(this Self& self) -> HBRUSH { return CreateSolidBrush(self.to_colorref()); }

auto color::to_winrt_color(this Self& self) -> winrt::Color {
    return winrt::Color { .A { self.a }, .R { self.r }, .G { self.g }, .B { self.b } };
}

auto color::to_webview2_color(this Self& self) -> COREWEBVIEW2_COLOR {
    return COREWEBVIEW2_COLOR { .A { self.a }, .R { self.r }, .G { self.g }, .B { self.b } };
}

auto color::is_dark(this Self& self) -> bool {
    return (((5 * self.g) + (2 * self.r) + self.b) < (8 * 128)) ? true : false;
}
} // namespace pane
