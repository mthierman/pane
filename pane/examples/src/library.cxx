#include <pane/pane.hxx>
#include <print>

auto main() -> int {
    auto lib_path { pane::file(
        L"C:\\Users\\mthie\\AppData\\Roaming\\Microsoft\\Windows\\Libraries\\Samples.library-ms") };

    std::println("Library: {}", lib_path.storage);

    auto load { lib_path.open_library() };

    if (load.has_value()) {
        std::println("Lib exists!");
        pane::file::library_directories(load.value());
    } else {
        std::println("{}", load.error().message());
    }

    // if (lib_path.load_library()) {
    //     std::println("Lib exists!");
    // }

    return 0;
}
