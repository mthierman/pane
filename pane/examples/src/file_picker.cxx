#include <Windows.h>
#include <cstdlib>
#include <format>
#include <print>

#include <pane/co_init.hxx>
#include <pane/window.hxx>
#include <pane/file_picker.hxx>
#include <pane/file.hxx>

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
        } else {
            OutputDebugStringA(
                pane::file::get_display_name(dir.value()).value_or(pane::string()).c_str());
        }
    }

    return pane::message_loop();
}
