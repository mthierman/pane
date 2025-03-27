#include <pane/pane.hxx>
#include <cstdlib>
#include <format>
#include <print>

auto main() -> int {
    auto mta { pane::co_init::multi_threaded() };

    std::println("{}", pane::hresult_error(mta.result));

    auto sta { pane::co_init::apartment_threaded() };

    switch (sta.result) {
        case S_OK: {
            std::println("S_OK");
        } break;
        case S_FALSE: {
            std::println("S_FALSE");
        } break;
        case RPC_E_CHANGED_MODE: {
            std::println("RPC_E_CHANGED_MODE");
        } break;
        default: {
            std::println("Default case, other error...");
        } break;
    }

    if (SUCCEEDED(mta)) {
        std::println("First call succeeded!");
    }

    return EXIT_SUCCESS;
}
