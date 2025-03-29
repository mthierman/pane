#include <pane/pane.hxx>
#include <cstdlib>
#include <format>
#include <print>

auto main() -> int {
    auto color { pane::color(0, 255, 0) };

    std::println("{}", color);

    return EXIT_SUCCESS;
}
