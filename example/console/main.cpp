#include <pane/pane.hpp>
#include <print>

// https://learn.microsoft.com/en-us/cpp/c-language/using-wmain
auto wmain(int /* argc */, wchar_t* /* argv */[], wchar_t* /* envp */[]) -> int {
    // auto args { pane::system::get_argv_u8(argc, argv) };

    // for (auto& arg : args) {
    //     std::println("{}", arg);
    // }

    auto text = std::u8string(u8"TEST");
    std::println("{}", text);

    return 0;
}
