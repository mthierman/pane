#include <pane/pane.hxx>
#include <print>

auto main() -> int {
    auto samples {
        L"C:\\Users\\mthie\\AppData\\Roaming\\Microsoft\\Windows\\Libraries\\Samples.library-ms",
    };

    auto lib_path { pane::file(samples) };
    std::println("{}", lib_path.storage);

    return 0;
}
