#include <cstdlib>
#include <format>
#include <print>
#include <pane/pane.hxx>

auto main() -> int {
    auto url { pane::url(u8"https://www.google.com/") };

    if (url) {
        std::println("{}", url.value().get_href());
    }

    return EXIT_SUCCESS;
}
