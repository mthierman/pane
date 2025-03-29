#include <Windows.h>
#include <pane/window.hxx>
#include <pane/message_box.hxx>

auto wWinMain(HINSTANCE, HINSTANCE, wchar_t*, int) -> int {
    auto window { pane::window(true) };

    return pane::system::message_loop();
}
