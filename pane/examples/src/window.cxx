#include <Windows.h>
#include <pane/window.hxx>
#include <pane/message_box.hxx>

auto wWinMain(HINSTANCE, HINSTANCE, wchar_t*, int) -> int {
    // auto window { pane::window(true) };

    // pane::message_box(std::string_view(std::format("TEST")), u8"TEST");
    // pane::message_box(std::format("TEST"), std::format("TEST"));

    pane::message_box(u8"TEST", u8"TEST");
    pane::message_box(std::u8string(u8"TEST"), std::u8string(u8"TEST"));

    std::u8string test { u8"TEST" };
    pane::message_box(test, test);

    return pane::system::message_loop();
}
