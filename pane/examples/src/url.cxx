#include <pane/pane.hxx>
#include <print>

auto main() -> int {
    // auto url { pane::url::parse(pane::string("https://www.google.com/").u8_str()) };
    auto url { pane::url::parse(u8"https://www.google.com/") };

    if (url) {
        std::println("{}", url.value().storage.get_href());
    }

    return 0;
}
