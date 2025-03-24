#include <pane/pane.hxx>
#include <print>

auto main() -> int {
    auto lib { pane::fs::library::create_from_name() };
    std::println("{}", pane::sys::format_message(lib));

    return 0;
}
