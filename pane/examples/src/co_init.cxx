#include <pane/pane.hxx>
#include <cstdlib>
#include <format>
#include <print>

auto main() -> int {
    auto co_init { pane::co_init::multithreaded() };

    std::println("{}", pane::hresult_error(co_init.result));

    return EXIT_SUCCESS;
}
