#include <pane/pane.hxx>
#include <print>

auto main() -> int {
    auto samples { pane::file(
        L"C:\\Users\\mthie\\AppData\\Roaming\\Microsoft\\Windows\\Libraries\\Samples.library-ms") };

    auto lib { samples.open_library() };

    if (lib) {
        auto dirs { pane::file::library_directories(lib.value()) };
        for (auto& dir : dirs) {
            std::println("{}", dir.storage);
        }
    } else {
        std::println("{}", lib.error().message());
    }

    return 0;
}
