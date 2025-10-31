#include <pane/pane.hpp>
#include <print>

// https://learn.microsoft.com/en-us/cpp/c-language/using-wmain
auto wmain(int argc, wchar_t* argv[], [[maybe_unused]] wchar_t* envp[]) -> int {
    auto args { pane::system::get_argv_u8(argc, argv) };

    for (auto& arg : args) {
        std::println("{}", arg);
    }

    return 0;
}
