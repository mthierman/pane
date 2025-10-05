#include <pane/pane.hpp>
#include "project.hpp"
#include <cstdlib>
#include <format>
#include <print>
#include <ranges>

// https://learn.microsoft.com/en-us/cpp/c-language/using-wmain
auto wmain(int /* argc */, wchar_t* /* argv */[], wchar_t* /* envp */[]) -> int {
    auto args { pane::system::command_line_arguments() };

    for (auto [idx, arg] : std::views::enumerate(args) | std::views::as_const) {
        std::println("{}: {}", idx, std::format("{}", arg));
    }

    std::println("{}, v{}", pane::PROJECT_NAME, pane::PROJECT_VERSION);

    return EXIT_SUCCESS;
}
