#include <Windows.h>
#include <pane/pane.hxx>

auto wWinMain(::HINSTANCE, ::HINSTANCE, wchar_t*, int) -> int {
    auto window { pane::window(true) };

    return pane::message_loop();
}
