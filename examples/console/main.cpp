#include <pane/pane.hpp>
#include <algorithm>
#include <cstdlib>
#include <print>

// https://learn.microsoft.com/en-us/cpp/c-language/using-wmain
auto wmain(int /* argc */, wchar_t* /* argv */[], wchar_t* /* envp */[]) -> int {
    std::println("{} v{} {} v{}",
                 pane::project_name,
                 pane::project_version,
                 pane::os_name,
                 pane::os_version);

    pane::system::argv argv;
    auto args { argv.as_u8string() };
    std::ranges::for_each(args, [idx = 0](const auto& arg) { std::println("{}: {}", idx, arg); });

    return EXIT_SUCCESS;
}
