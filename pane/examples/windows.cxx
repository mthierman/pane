#include <pane/pane.hxx>

// https://learn.microsoft.com/en-us/windows/win32/learnwin32/winmain--the-application-entry-point
auto WINAPI wWinMain(HINSTANCE /* hinstance */,
                     HINSTANCE /* hprevinstance */,
                     PWSTR /* pcmdline */,
                     int /* ncmdshow */) -> int {
    // auto webview { pane::webview({}, { .home_page = u8"https://www.google.com/" }) };

    // auto window { pane::window() };
    // window.~window();

    // auto win { std::make_unique<pane::window>() };

    auto window { pane::window({ .title { u8"pane" },
                                 .background_color { pane::color { 0, 0, 0, 0 } },
                                 .visible { false },
                                 .shutdown { false } },
                               [&](HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> LRESULT {
        if (msg == WM_SETTINGCHANGE) {
            //
        }

        return DefWindowProcW(hwnd, msg, wparam, lparam);
    }) };

    return pane::system::message_loop();
}
