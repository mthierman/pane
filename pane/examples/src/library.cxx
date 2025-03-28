#include <cstdlib>
#include <format>
#include <print>

#include <pane/co_init.hxx>
#include <pane/file.hxx>

auto main() -> int {
    auto co_init { pane::co_init::multi_threaded() };

    auto samples { pane::file(
        L"C:\\Users\\mthie\\AppData\\Roaming\\Microsoft\\Windows\\Libraries\\Samples.library-ms") };

    auto lib { samples.open_library() };

    if (lib) {
        auto dirs { pane::file::library_directories(lib.value()) };

        if (dirs) {
            for (auto& dir : dirs.value()) {
                std::println("{}", dir.storage);
            }
        }
    } else {
        std::println("{}", lib.error().message());
    }

    return EXIT_SUCCESS;
}
