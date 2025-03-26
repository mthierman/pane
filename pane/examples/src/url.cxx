#include <pane/pane.hxx>
#include <print>

auto main() -> int {
    auto url { pane::url::from_string(pane::string("https://www.google.com/")) };

    if (url) {
        std::println("{}", url.value().storage.get_href());
    }

    return 0;
}
