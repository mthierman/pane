#include <Windows.h>

#include <pane/window.hxx>
#include <pane/system.hxx>
#include <pane/console.hxx>

#include <pane/debug.hxx>
#include <pane/message_box.hxx>

#include <string>

auto wWinMain(HINSTANCE, HINSTANCE, wchar_t*, int) -> int {
    // auto console { pane::console() };

    // auto window { pane::window(true) };

    std::string narrow1 { "TEST1" };
    std::string narrow2 { "TEST2" };

    pane::debug("{}, {}", narrow1, narrow2);

    std::wstring wide1 { L"TEST1" };
    std::wstring wide2 { L"TEST2" };

    pane::debug(L"{}, {}", wide1, wide2);

    pane::debug_message("{}, {}", narrow1, narrow2);
    pane::debug_message(L"{}, {}", wide1, wide2);

    return pane::message_loop();
}
