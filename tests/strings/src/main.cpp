#include <pane/pane.hpp>
#include <print>
#include <filesystem>

auto wmain(int argc, wchar_t* argv[], [[maybe_unused]] wchar_t* envp[]) -> int {
    auto test { u8"test" };
    auto u8string { std::u8string { test } };

    std::println("{}", test);
    std::println("{}", u8string);

    return 0;
}
