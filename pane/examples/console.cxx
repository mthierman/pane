#include <pane/pane.hxx>
#include <cstdlib>
#include <format>
#include <print>

auto wmain(int /* argc */, wchar_t* /* argv */[], wchar_t* /* envp */[]) -> int {
    std::println("SUBSYSTEM:CONSOLE");

    return EXIT_SUCCESS;
}
