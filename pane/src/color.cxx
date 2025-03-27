#include <pane/color.hxx>
#include <limits>

namespace pane {
color::color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = std::numeric_limits<uint8_t>::max())
    : r { r },
      g { g },
      b { b },
      a { a } { }
} // namespace pane
