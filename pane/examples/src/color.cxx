#include <cstdlib>
#include <format>
#include <print>

#include <pane/color.hxx>

auto main() -> int {
    auto color { pane::color(0, 255, 0) };

    std::println("R: {} G: {} B: {} A: {}", color.r, color.g, color.b, color.a);
    std::println("{}", color);
    std::println("{}", color.to_hex());

    return EXIT_SUCCESS;
}
