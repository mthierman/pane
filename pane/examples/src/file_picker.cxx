#include <Windows.h>
#include <cstdlib>
#include <format>
#include <print>
#include <pane/pane.hxx>

auto wWinMain(HINSTANCE, HINSTANCE, wchar_t*, int) -> int {
    auto co_init { pane::co_init::apartment_threaded() };

    auto window { pane::window(true) };

    auto file_picker { pane::file_picker() };

    auto dir { file_picker.open_directory() };

    if (dir) {
        // OutputDebugStringW(dir.value()->GetDisplayName())
        auto lib { pane::file::open_library(dir.value()) };

        if (lib) {
            auto dirs { pane::file::library_directories(lib.value()) };

            if (dirs) {
                auto files = dirs.value();

                for (auto& file : files) {
                    OutputDebugStringA(std::format("{}\n", file.storage).c_str());
                }
            }
        }
    }

    return pane::message_loop();
}
