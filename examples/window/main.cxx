#include <pane/pane.hxx>
#include <memory>
#include <vector>

enum struct message : int { NEW_WINDOW = WM_APP, CLOSE_WINDOW };

// https://learn.microsoft.com/en-us/windows/win32/learnwin32/winmain--the-application-entry-point
auto wWinMain(HINSTANCE /* hinstance */,
              HINSTANCE /* hprevinstance */,
              PWSTR /* pcmdline */,
              int /* ncmdshow */) -> int {
    pane::window_manager<pane::window> window_manager;

    pane::message_window app { [&](const pane::window_message& window_message,
                                   pane::message_window& app) -> LRESULT {
        switch (window_message.msg) {
            case +message::NEW_WINDOW: {
                window_manager.add(pane::window_config { u8"windows",
                                                         pane::color { 0, 0, 0, 255 },
                                                         pane::color { 255, 255, 255, 255 },
                                                         true,
                                                         nullptr },
                                   [&](const pane::window_message& window_message,
                                       pane::window& window) -> LRESULT {
                    switch (window_message.msg) {
                        case WM_CREATE: {
                        } break;

                        case WM_CLOSE: {
                            pane::window_message { app.window_handle(),
                                                   +message::CLOSE_WINDOW,
                                                   0,
                                                   (LPARAM)window.window_handle() }
                                .send();
                        } break;

                        case WM_KEYDOWN: {
                            switch (window_message.wparam) {
                                case 'N': {
                                    if (pane::input::is_key_down(VK_LCONTROL)
                                        || pane::input::is_key_down(VK_RCONTROL)) {
                                        pane::window_message {
                                            app.window_handle(), +message::NEW_WINDOW, 0, 0
                                        }
                                            .send();
                                    }
                                } break;
                                case 'W': {
                                    if (pane::input::is_key_down(VK_LCONTROL)
                                        || pane::input::is_key_down(VK_RCONTROL)) {
                                        pane::window_message { app.window_handle(),
                                                               +message::CLOSE_WINDOW,
                                                               0,
                                                               (LPARAM)window.window_handle() }
                                            .send();
                                    }
                                } break;
                            }
                        } break;
                    }

                    return window.default_procedure(window_message);
                });
            } break;

            case +message::CLOSE_WINDOW: {
                window_manager.remove((HWND)window_message.lparam);
            } break;
        }

        return app.default_procedure(window_message);
    } };

    pane::window_message { app.window_handle(), +message::NEW_WINDOW, 0, 0 }.send();

    return pane::system::message_loop();
}
