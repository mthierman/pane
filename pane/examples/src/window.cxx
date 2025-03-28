#include <Windows.h>

#include <pane/window.hxx>
#include <pane/system.hxx>
#include <pane/console.hxx>

auto wWinMain(HINSTANCE, HINSTANCE, wchar_t*, int) -> int {
    auto console { pane::console() };

    auto window { pane::window(true) };

    return pane::message_loop();
}
