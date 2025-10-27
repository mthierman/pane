#include <pane/pane.hpp>
#include <cstdlib>
#include <print>
#include <ranges>

// https://learn.microsoft.com/en-us/cpp/c-language/using-wmain
auto wmain(int argc, wchar_t** argv, wchar_t** /* envp */) -> int {
    auto args { pane::system::get_argv_u8(argc, argv) };

    for (auto [idx, arg] : std::views::enumerate(args)) {
        std::println("{}: {}", idx, arg);
    }

    return EXIT_SUCCESS;
}
