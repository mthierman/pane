#include <pane/pane.hxx>

struct window : pane::window<window> {
    using Self = window;
    using pane::window<window>::window;

    static auto config() -> config {
        return { u8"window_manager",
                 pane::color { 0, 0, 0, 0 },
                 pane::color { 0, 0, 0, 0 },
                 true,
                 nullptr };
    }

    auto handle_message(this Self& self, const pane::window_message& window_message) -> LRESULT {
        switch (window_message.msg) {
            case WM_CLOSE: {
                self.window_manager->destroy(self);
            } break;

            case WM_KEYDOWN: {
                switch (window_message.wparam) {
                    case 'N': {
                        if (pane::input::is_key_down(VK_LCONTROL)
                            || pane::input::is_key_down(VK_RCONTROL)) {
                            self.window_manager->create();
                        }
                    } break;
                    case 'W': {
                        if (pane::input::is_key_down(VK_LCONTROL)
                            || pane::input::is_key_down(VK_RCONTROL)) {
                            SendMessageW(window_message.hwnd, WM_CLOSE, 0, 0);
                        }
                    } break;
                }
            } break;
        }

        return self.default_procedure(window_message);
    }
};

// https://learn.microsoft.com/en-us/windows/win32/learnwin32/winmain--the-application-entry-point
auto wWinMain(HINSTANCE /* hinstance */,
              HINSTANCE /* hprevinstance */,
              PWSTR /* pcmdline */,
              int /* ncmdshow */) -> int {
    pane::window_manager<window> window_manager;

    return pane::system::message_loop();
}
