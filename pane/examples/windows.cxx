#include <pane/pane.hxx>

// https://learn.microsoft.com/en-us/windows/win32/learnwin32/winmain--the-application-entry-point
auto wWinMain(HINSTANCE /* hinstance */,
              HINSTANCE /* hprevinstance */,
              PWSTR /* pcmdline */,
              int /* ncmdshow */) -> int {
    auto window_manager { pane::window_manager() };

    auto window { pane::window(
        { u8"window", pane::color { 0, 0, 0, 255 }, true, nullptr },
        [&](pane::window* window, pane::window_message window_message) -> LRESULT {
        switch (window_message.msg) {
            case WM_CREATE: {
                window_manager.insert(window->window_handle);
            } break;
            case WM_DESTROY: {
                window_manager.erase(window->window_handle);
            } break;
        }

        return window_message.default_procedure();
    }) };

    auto webview { pane::webview(
        { u8"webview", pane::color { 0, 0, 0, 255 }, true, nullptr },
        { .home_page = u8"https://www.google.com/" },
        [&](pane::webview* webview, pane::window_message window_message) -> LRESULT {
        switch (window_message.msg) {
            case WM_CREATE: {
                window_manager.insert(webview->window_handle);
            } break;
            case WM_DESTROY: {
                window_manager.erase(webview->window_handle);
            } break;
        }

        return window_message.default_procedure();
    }) };

    return pane::system::message_loop();
}
