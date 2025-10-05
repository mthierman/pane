#include <pane/pane.hpp>
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

    std::println("{}, v{}", pane::project_name, pane::project_version);
    std::println("{}, v{}", pane::os_name, pane::os_version);

    pane::debug_print("{}", u8"1");
    pane::debug_print("{}", u8"2");
    pane::debug_print("{}", u8"3");

    pane::debug_println("{}", u8"println");
    pane::debug_println("{}", u8"println");

    pane::messagebox("{}", u8"test");

    return EXIT_SUCCESS;
}
