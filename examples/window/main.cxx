#include <pane/pane.hxx>

struct test_window : pane::window<test_window> {
    using base = pane::window<test_window>;
    using base::base;

    auto message_handler(const pane::window_message& window_message) -> LRESULT {
        switch (window_message.msg) {
            case WM_CLOSE: {
                // window_manager.destroy(window.window_handle());
                PostQuitMessage(0);
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
                            // SendMessageW(window.window_handle(), WM_CLOSE, 0, 0);
                        }
                    } break;
                }
            } break;
        }

        return window_message.default_procedure();
    }
};

// https://learn.microsoft.com/en-us/windows/win32/learnwin32/winmain--the-application-entry-point
auto wWinMain(HINSTANCE /* hinstance */,
              HINSTANCE /* hprevinstance */,
              PWSTR /* pcmdline */,
              int /* ncmdshow */) -> int {
    test_window window { { u8"window",
                           pane::color { 0, 0, 0, 255 },
                           pane::color { 255, 255, 255, 255 },
                           true,
                           nullptr } };

    return pane::system::message_loop();
}
