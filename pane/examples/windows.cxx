#include <pane/pane.hxx>

auto make_window() -> pane::window {
    return pane::window({ u8"pane", pane::color { 0, 0, 0, 0 }, true, true },
                        [&](pane::window::message message) -> LRESULT {
        if (message.msg == WM_CREATE) {
            pane::debug("WM_CREATE");
        }

        return DefWindowProcW(message.hwnd, message.msg, message.wparam, message.lparam);
    });
}

auto make_webview() -> pane::webview {
    return pane::webview({},
                         { .home_page = u8"https://www.google.com/" },
                         [&](pane::window::message message) -> LRESULT {
        if (message.msg == WM_CREATE) {
            pane::debug("WM_CREATE");
        }

        return DefWindowProcW(message.hwnd, message.msg, message.wparam, message.lparam);
    });
}

// https://learn.microsoft.com/en-us/windows/win32/learnwin32/winmain--the-application-entry-point
auto WINAPI wWinMain(HINSTANCE /* hinstance */,
                     HINSTANCE /* hprevinstance */,
                     PWSTR /* pcmdline */,
                     int /* ncmdshow */) -> int {
    auto window { make_window() };
    auto window2 { make_window() };
    auto webview { make_webview() };

    return pane::system::message_loop();
}
