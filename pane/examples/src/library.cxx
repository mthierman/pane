#include <pane/pane.hxx>
#include <print>
#include <wil/resource.h>

auto main() -> int {
    // auto init { wil::CoInitializeEx() };
    // auto lib { pane::fs::library::create_from_name() };
    // std::println("{}", pane::sys::format_message(lib));

    auto init { wil::CoInitializeEx() };
    auto lib { pane::library::create() };
    auto folders { pane::library::get_folders(lib) };
    // std::println("{}", pane::sys::format_message(lib));

    return 0;
}
