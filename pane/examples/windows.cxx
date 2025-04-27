#include <pane/pane.hxx>

auto make_window(pane::window_manager& window_manager) -> pane::window {
    return pane::window({ u8"pane", pane::color { 255, 0, 0, 255 }, true, false },
                        [&](pane::window::procedure procedure) -> LRESULT {
        switch (procedure.msg.msg) {
            case WM_CREATE: {
                window_manager.insert(procedure.msg.hwnd);
            } break;
            case WM_DESTROY: {
                window_manager.erase(procedure.msg.hwnd);
            } break;
        }

        return procedure.msg.default_procedure();
    });
}

auto make_webview(pane::window_manager& window_manager) -> pane::webview {
    return pane::webview({ .background_color = pane::color { 16, 18, 24, 255 } },
                         { .home_page = u8"https://www.google.com/" },
                         [&](pane::webview::procedure procedure) -> LRESULT {
        switch (procedure.msg.msg) {
            case WM_CREATE: {
                window_manager.insert(procedure.msg.hwnd);
            } break;
            case WM_DESTROY: {
                window_manager.erase(procedure.msg.hwnd);
            } break;
        }

        return procedure.msg.default_procedure();
    });
}

// https://learn.microsoft.com/en-us/windows/win32/learnwin32/winmain--the-application-entry-point
auto WINAPI wWinMain(HINSTANCE /* hinstance */,
                     HINSTANCE /* hprevinstance */,
                     PWSTR /* pcmdline */,
                     int /* ncmdshow */) -> int {
    auto window_manager { pane::window_manager() };

    // auto window { make_window(window_manager) };
    auto webview { make_webview(window_manager) };

    return pane::system::message_loop();
}
