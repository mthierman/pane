#include <pane/pane.hxx>
#include <cstdlib>
#include <format>
#include <print>

auto main() -> int {
    auto mta { pane::co_init::multi_threaded() };

    std::println("{}", pane::hresult_error(mta.result));

    auto sta { pane::co_init::apartment_threaded() };

    std::println("{}", pane::hresult_error(sta.result));

    return EXIT_SUCCESS;
}
