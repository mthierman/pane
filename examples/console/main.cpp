#include <pane/pane.hpp>
#include <cstdlib>
#include <format>
#include <print>
#include <ranges>

// https://learn.microsoft.com/en-us/cpp/c-language/using-wmain
auto wmain(int /* argc */, wchar_t* /* argv */[], wchar_t* /* envp */[]) -> int {
    auto args { pane::system::command_line_arguments() };

    std::println("{} v{} {} v{}",
                 pane::project_name,
                 pane::project_version,
                 pane::os_name,
                 pane::os_version);

    for (auto [idx, arg] : std::views::enumerate(args) | std::views::as_const) {
        std::println("{}: {}", idx, arg);
    }

    return EXIT_SUCCESS;
}
