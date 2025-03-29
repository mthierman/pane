#include <pane/pane.hxx>

// https://learn.microsoft.com/en-us/windows/win32/learnwin32/winmain--the-application-entry-point
auto wWinMain(HINSTANCE, HINSTANCE, wchar_t*, int) -> int {
    auto window { pane::window(true) };

    return pane::system::message_loop();
}
