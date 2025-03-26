#include <pane/pane.hxx>
#include <print>

auto main() -> int {
    auto init { wil::CoInitializeEx() };

    auto samples {
        L"C:\\Users\\mthie\\AppData\\Roaming\\Microsoft\\Windows\\Libraries\\Samples.library-ms",
    };

    // auto samples {
    //     L"Test",
    // };

    auto lib_path { pane::file(samples) };
    std::println("{}", lib_path.storage);

    auto load { lib_path.load_library() };

    if (load.has_value()) {
        std::println("Lib exists!");
        pane::file::get_folders(load.value());
    } else {
        std::println("{}", load.error().message());
    }

    // if (lib_path.load_library()) {
    //     std::println("Lib exists!");
    // }

    return 0;
}
