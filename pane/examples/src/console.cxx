#include <pane/pane.hxx>
#include <type_traits>
#include <print>
#include <system_error>

auto main() -> int {
    auto path { pane::path() };

    std::println("{}", path.storage.empty());

    return 0;
}
