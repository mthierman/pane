#include <pane/pane.hxx>

// https://learn.microsoft.com/en-us/windows/win32/learnwin32/winmain--the-application-entry-point
auto wWinMain(HINSTANCE /* hinstance */,
              HINSTANCE /* hprevinstance */,
              PWSTR /* pcmdline */,
              int /* ncmdshow */) -> int {
    pane::window_manager window_manager;

    pane::window main_window { { u8"window",
                                 pane::color { 0, 0, 0, 255 },
                                 pane::color { 255, 255, 255, 255 },
                                 true,
                                 nullptr },
                               [&](pane::window* window,
                                   pane::window_message window_message) -> LRESULT {
        switch (window_message.event) {
            case WM_CREATE: {
                window_manager.insert(window->window_handle);
            } break;
            case WM_DESTROY: {
                window_manager.erase(window->window_handle);
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

        return window->default_procedure(window_message);
    } };

    return pane::system::message_loop();
}
