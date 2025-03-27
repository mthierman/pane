#include <pane/pane.hxx>
#include <cstdlib>
#include <format>
#include <print>

auto main() -> int {
    auto url { pane::url::create(u8"https://www.google.com/") };

    if (url) {
        std::println("{}", url.value().storage.get_href());
    }

    return EXIT_SUCCESS;
}
