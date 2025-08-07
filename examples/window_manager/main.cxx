#include <pane/pane.hxx>

struct window : pane::window<window> {
    using Self = window;
    using pane::window<window>::window;

    auto message_handler(this Self& self, const pane::window_message& window_message) -> LRESULT {
        switch (window_message.msg) {
            case WM_DESTROY: {
                pane::system::quit();

                return 0;
            } break;

            case WM_KEYDOWN: {
                switch (window_message.wparam) {
                    case 'N': {
                        if (pane::input::is_key_down(VK_LCONTROL)
                            || pane::input::is_key_down(VK_RCONTROL)) {
                            // window_manager.create();
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
    window window { { u8"window",
                      pane::color { 0, 0, 0, 255 },
                      pane::color { 255, 255, 255, 255 },
                      true,
                      nullptr } };

    return pane::system::message_loop();
}
