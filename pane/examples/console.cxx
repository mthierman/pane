#include <pane/pane.hxx>
#include <cstdlib>
#include <format>
#include <print>

// https://learn.microsoft.com/en-us/cpp/c-language/using-wmain
auto wmain(int /* argc */, wchar_t* /* argv */[], wchar_t* /* envp */[]) -> int {
    auto args { pane::system::command_line_arguments() };

    for (auto idx = 0; auto& arg : args) {
        std::println("{}: {}", idx, std::format("{}", arg));
        ++idx;
    }

    return EXIT_SUCCESS;
}
