#include <pane/color.hxx>
#include <format>

namespace pane {
color::color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
    : r { r },
      g { g },
      b { b },
      a { a } { }

auto color::to_hex(this Self& self) -> string {
    return string(std::format("#{:0>2x}{:0>2x}{:0>2x}{:0>2x}", self.r, self.g, self.b, self.a));
}

auto color::to_colorref(this Self& self) -> COLORREF { return RGB(self.r, self.g, self.b); }

auto color::to_hbrush(this Self& self) -> HBRUSH { return CreateSolidBrush(self.to_colorref()); }

auto color::to_winrt_color(this Self& self) -> winrt::Color {
    return winrt::Color { .A { self.a }, .R { self.r }, .G { self.g }, .B { self.b } };
}
} // namespace pane
