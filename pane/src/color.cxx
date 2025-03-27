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

auto color::to_hbrush(this Self& self) -> HBRUSH {
    return CreateSolidBrush(RGB(self.r, self.g, self.b));
}
} // namespace pane
