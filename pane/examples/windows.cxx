#include <pane/pane.hxx>

// https://learn.microsoft.com/en-us/windows/win32/learnwin32/winmain--the-application-entry-point
auto wWinMain(HINSTANCE /* hinstance */,
              HINSTANCE /* hprevinstance */,
              PWSTR /* pcmdline */,
              int /* ncmdshow */) -> int {
    auto window_manager { pane::window_manager<pane::window>() };
    auto webview_manager { pane::window_manager<pane::webview>() };

    auto window { pane::window(
        { u8"test", pane::color { 0, 255, 0, 255 }, true, nullptr },
        [&](pane::window* window, pane::window_message window_message) -> LRESULT {
        switch (window_message.msg) {
            case WM_CREATE: {
                window_manager.insert(window);
            } break;
            case WM_DESTROY: {
                window_manager.erase(window);
            } break;
        }

        return window_message.default_procedure();
    }) };

    auto webview { pane::webview(
        { .background_color = pane::color { 16, 18, 24, 255 } },
        { .home_page = u8"https://www.google.com/" },
        [&](pane::webview* webview, pane::window_message window_message) -> LRESULT {
        switch (window_message.msg) {
            case WM_CREATE: {
                webview_manager.insert(webview);
            } break;
            case WM_DESTROY: {
                webview_manager.erase(webview);
            } break;
        }

        return window_message.default_procedure();
    }) };

    return pane::system::message_loop();
}
