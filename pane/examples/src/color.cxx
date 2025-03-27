#include <pane/pane.hxx>
#include <cstdlib>
#include <format>
#include <print>

auto main() -> int {
    //
    auto color { pane::color() };

    std::println("R: {} G: {} B: {} A: {}", color.r, color.g, color.b, color.a);

    return EXIT_SUCCESS;
}
