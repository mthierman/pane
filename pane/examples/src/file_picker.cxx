#include <Windows.h>
#include <cstdlib>
#include <format>
#include <print>
#include <pane/pane.hxx>

auto wWinMain(HINSTANCE, HINSTANCE, wchar_t*, int) -> int {
    auto co_initialize { wil::CoInitializeEx(COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE) };

    auto window { pane::window(true) };

    return pane::message_loop();
}
