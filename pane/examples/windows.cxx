#include <pane/pane.hxx>

auto make_window(pane::window_manager& window_manager) -> pane::window {
    return pane::window({ u8"pane", pane::color { 255, 0, 0, 255 }, true, false },
                        [&](pane::window::message message) -> LRESULT {
        if (message.msg == WM_CREATE) {
            window_manager.insert(message.window->window_handle);
        }

        if (message.msg == WM_DESTROY) {
            window_manager.erase(message.window->window_handle);
        }

        return DefWindowProcW(
            message.window->window_handle, message.msg, message.wparam, message.lparam);
    });
}

auto make_webview(pane::window_manager& window_manager) -> pane::webview {
    return pane::webview({ .background_color = pane::color { 0, 0, 255, 255 } },
                         { .home_page = u8"https://www.google.com/" },
                         [&](pane::webview::message message) -> LRESULT {
        if (message.msg == WM_CREATE) {
            window_manager.insert(message.webview->window_handle);
        }

        if (message.msg == WM_DESTROY) {
            window_manager.erase(message.webview->window_handle);
        }

        return DefWindowProcW(
            message.webview->window_handle, message.msg, message.wparam, message.lparam);
    });
}

// https://learn.microsoft.com/en-us/windows/win32/learnwin32/winmain--the-application-entry-point
auto WINAPI wWinMain(HINSTANCE /* hinstance */,
                     HINSTANCE /* hprevinstance */,
                     PWSTR /* pcmdline */,
                     int /* ncmdshow */) -> int {
    auto window_manager { pane::window_manager() };

    auto window { make_window(window_manager) };
    auto webview { make_webview(window_manager) };

    return pane::system::message_loop();
}
