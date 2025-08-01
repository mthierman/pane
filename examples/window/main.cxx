#include <pane/pane.hxx>

// https://learn.microsoft.com/en-us/windows/win32/learnwin32/winmain--the-application-entry-point
auto wWinMain(HINSTANCE /* hinstance */,
              HINSTANCE /* hprevinstance */,
              PWSTR /* pcmdline */,
              int /* ncmdshow */) -> int {
    pane::window_manager<pane::window> window_manager {
        { u8"window",
          pane::color { 0, 0, 0, 255 },
          pane::color { 255, 255, 255, 255 },
          true,
          nullptr },
        [&](const pane::window_message& window_message, pane::window& window) -> LRESULT {
        switch (window_message.msg) {
            case WM_CLOSE: {
                window_manager.remove(window.window_handle());
            } break;

            case WM_KEYDOWN: {
                switch (window_message.wparam) {
                    case 'N': {
                        if (pane::input::is_key_down(VK_LCONTROL)
                            || pane::input::is_key_down(VK_RCONTROL)) {
                            window_manager.add();
                        }
                    } break;
                    case 'W': {
                        if (pane::input::is_key_down(VK_LCONTROL)
                            || pane::input::is_key_down(VK_RCONTROL)) {
                            SendMessageW(window.window_handle(), WM_CLOSE, 0, 0);
                        }
                    } break;
                }
            } break;
        }

        return window.default_procedure(window_message);
    }
    };

    return pane::system::message_loop();
}
