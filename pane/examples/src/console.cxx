#include <pane/pane.hxx>
#include <type_traits>
#include <print>
#include <system_error>

auto main() -> int {
    auto string { pane::string(u8"test") };
    auto path { pane::path(string) };

    std::println("{}", path.storage);

    return 0;
}
