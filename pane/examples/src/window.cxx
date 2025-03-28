#include <Windows.h>

#include <pane/window.hxx>
#include <pane/system.hxx>

auto wWinMain(HINSTANCE, HINSTANCE, wchar_t*, int) -> int {
    auto window { pane::window(true) };

    return pane::message_loop();
}
