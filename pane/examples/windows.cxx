#include <pane/pane.hxx>

// https://learn.microsoft.com/en-us/windows/win32/learnwin32/winmain--the-application-entry-point
auto WINAPI wWinMain(HINSTANCE /* hinstance */,
                     HINSTANCE /* hprevinstance */,
                     PWSTR /* pcmdline */,
                     int /* ncmdshow */) -> int {
    auto window_manager { pane::window_manager() };

    auto window { pane::window({ .background_color = pane::color { 16, 18, 24, 255 } },
                               [&](pane::window* window, pane::window_message msg) -> LRESULT {
        switch (msg.msg) {
            case WM_CREATE: {
                window_manager.insert(window->window_handle.hwnd);
            } break;
            case WM_DESTROY: {
                window_manager.erase(window->window_handle.hwnd);
            } break;
        }

        return msg.default_procedure();
    }) };

    auto webview { pane::webview({ .background_color = pane::color { 16, 18, 24, 255 } },
                                 { .home_page = u8"https://www.google.com/" },
                                 [&](pane::webview* window, pane::window_message msg) -> LRESULT {
        switch (msg.msg) {
            case WM_CREATE: {
                window_manager.insert(window->window_handle.hwnd);
            } break;
            case WM_DESTROY: {
                window_manager.erase(window->window_handle.hwnd);
            } break;
        }

        return msg.default_procedure();
    }) };

    return pane::system::message_loop();
}
