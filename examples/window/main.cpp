#include <pane/pane.hpp>

struct window : pane::window<window> {
    using Self = window;
    using pane::window<window>::window;

    static auto config() -> config {
        return {
            u8"window", pane::color { 0, 0, 0, 255 }, pane::color { 0, 0, 0, 255 }, true, nullptr
        };
    }

    auto handle_message(this Self& self, const pane::window_message& window_message) -> LRESULT {
        return self.default_procedure(window_message);
    }
};

// https://learn.microsoft.com/en-us/windows/win32/learnwin32/winmain--the-application-entry-point
auto wWinMain(HINSTANCE /* hinstance */,
              HINSTANCE /* hprevinstance */,
              PWSTR /* pcmdline */,
              int /* ncmdshow */) -> int {
    window window;

    return pane::system::message_loop();
}
