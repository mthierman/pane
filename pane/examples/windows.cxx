#include <pane/pane.hxx>

// https://learn.microsoft.com/en-us/windows/win32/learnwin32/winmain--the-application-entry-point
auto WINAPI wWinMain(HINSTANCE /* hinstance */,
                     HINSTANCE /* hprevinstance */,
                     PWSTR /* pcmdline */,
                     int /* ncmdshow */) -> int {
    // auto window { pane::window() };
    // window.~window();

    // auto win { std::make_unique<pane::window>() };

    auto window { pane::window({ u8"pane", pane::color { 0, 0, 0, 0 }, true, true },
                               [&](pane::window::message message) -> LRESULT {
        if (message.msg == WM_WINDOWPOSCHANGED) {
            pane::debug("WM_WINDOWPOSCHANGED");
        }

        return DefWindowProcW(message.hwnd, message.msg, message.wparam, message.lparam);
    }) };

    auto webview { pane::webview({},
                                 { .home_page = u8"https://www.google.com/" },
                                 [&](pane::window::message message) -> LRESULT {
        if (message.msg == WM_WINDOWPOSCHANGED) {
            pane::debug("WM_WINDOWPOSCHANGED");
        }

        return DefWindowProcW(message.hwnd, message.msg, message.wparam, message.lparam);
    }) };

    return pane::system::message_loop();
}
