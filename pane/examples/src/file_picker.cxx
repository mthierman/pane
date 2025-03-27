#include <Windows.h>
#include <cstdlib>
#include <format>
#include <print>
#include <pane/pane.hxx>

auto wWinMain(HINSTANCE, HINSTANCE, wchar_t*, int) -> int {
    auto co_init { pane::co_init_apartment() };

    auto window { pane::window(true) };

    return pane::message_loop();
}
