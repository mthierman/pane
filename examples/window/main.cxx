#include <pane/pane.hxx>

enum struct message : int {
    NOTIFY = WM_APP,
};

// https://learn.microsoft.com/en-us/windows/win32/learnwin32/winmain--the-application-entry-point
auto wWinMain(HINSTANCE /* hinstance */,
              HINSTANCE /* hprevinstance */,
              PWSTR /* pcmdline */,
              int /* ncmdshow */) -> int {
    pane::window_manager window_manager;
    pane::message_window message_window { [&](const pane::window_message& window_message,
                                              pane::message_window* message_window) -> LRESULT {
        auto& self = *message_window;

        if (window_message.msg == +message::NOTIFY) {
            pane::debug("message::NOTIFY");
        }

        return self.default_procedure(window_message);
    } };

    auto window { pane::window {
        { u8"window",
          pane::color { 0, 0, 0, 255 },
          pane::color { 255, 255, 255, 255 },
          true,
          nullptr },
        [&](const pane::window_message& window_message, pane::window* window) -> LRESULT {
        auto& self = *window;

        switch (window_message.msg) {
            case WM_CREATE: {
                window_manager.insert(self.window_handle);

                pane::window_message { message_window.window_handle(), +message::NOTIFY, 0, 0 }
                    .send();
            } break;

            case WM_DESTROY: {
                window_manager.erase(self.window_handle);
            } break;

            case WM_KEYDOWN: {
                switch (window_message.wparam) {
                    case 'N': {
                        if (pane::input::is_key_down(VK_LCONTROL)
                            || pane::input::is_key_down(VK_RCONTROL)) {
                            pane::debug("Ctrl+N");
                        } else {
                            pane::debug("N");
                        }
                    } break;
                }
            } break;
        }

        return self.default_procedure(window_message);
    } } };

    return pane::system::message_loop();
}
