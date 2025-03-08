#include <pane/pane.hxx>
#include <print>

auto main() -> int {
    auto string { pane::string() };
    auto from { std::u8string(u8"TEST") };

    std::println("{}", from);

    string = from;

    std::println("{}", from);

    return 0;
}
