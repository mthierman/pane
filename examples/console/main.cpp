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

    auto raw_u8string { u8"test" };
    std::u8string u8string { raw_u8string };

    std::println("{}", std::u8string_view { raw_u8string });
    std::println("{}", u8string);
    std::println("{}", std::u8string_view { u8string });

    // std::println("{}", u8"test");
    // auto fmt { std::format("{}", u8"test", u8"unicode") };
    // auto u8fmt { std::u8string { fmt.begin(), fmt.end() } };
    // auto u8view { std::u8string_view { reinterpret_cast<const char8_t*>(fmt.data()), fmt.size() }
    // };

    return EXIT_SUCCESS;
}
