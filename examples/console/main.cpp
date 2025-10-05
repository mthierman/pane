#include <pane/pane.hpp>
#include <cstdlib>
#include <format>
#include <print>
#include <ranges>

struct my_struct {
    int i = 287;
    double d = 3.14;
    std::u8string hello = u8"Hello World";
    std::array<uint64_t, 3> arr = { 1, 2, 3 };
    std::map<std::string, int> map { { "one", 1 }, { "two", 2 } };
};

// https://learn.microsoft.com/en-us/cpp/c-language/using-wmain
auto wmain(int /* argc */, wchar_t* /* argv */[], wchar_t* /* envp */[]) -> int {
    auto args { pane::system::command_line_arguments() };

    for (auto [idx, arg] : std::views::enumerate(args) | std::views::as_const) {
        std::println("{}: {}", idx, std::format("{}", arg));
    }

    std::println("{}, v{}", pane::project_name, pane::project_version);
    std::println("{}, v{}", pane::os_name, pane::os_version);

    my_struct s {};
    std::string buffer {};
    auto ec = glz::write_json(s, buffer);
    if (ec) {
        // handle error
    }

    std::println("{}", buffer);

    return EXIT_SUCCESS;
}
