#include <pane/pane.hxx>

// https://learn.microsoft.com/en-us/windows/win32/learnwin32/winmain--the-application-entry-point
auto wWinMain(HINSTANCE /* hinstance */,
              HINSTANCE /* hprevinstance */,
              PWSTR /* pcmdline */,
              int /* ncmdshow */) -> int {
    auto window_manager { pane::window_manager() };

    auto window { pane::window(
        { .background_color = pane::color { 16, 18, 24, 255 } },
        [&window_manager](pane::window* window, pane::window_message msg) -> LRESULT {
        switch (msg.msg) {
            case WM_CREATE: {
                window_manager.insert(window->window_handle);
            } break;
            case WM_DESTROY: {
                window_manager.erase(window->window_handle);
            } break;
        }

        return msg.default_procedure();
    }) };

    auto webview { pane::webview(
        { .background_color = pane::color { 16, 18, 24, 255 } },
        { .home_page = u8"https://www.google.com/" },
        [&window_manager](pane::webview* webview, pane::window_message msg) -> LRESULT {
        switch (msg.msg) {
            case WM_CREATE: {
                window_manager.insert(webview->window_handle);
            } break;
            case WM_DESTROY: {
                window_manager.erase(webview->window_handle);
            } break;
        }

        return msg.default_procedure();
    }) };

    return pane::system::message_loop();
}
