#include <pane/pane.hxx>

// https://learn.microsoft.com/en-us/windows/win32/learnwin32/winmain--the-application-entry-point
auto wWinMain(HINSTANCE /* hinstance */,
              HINSTANCE /* hprevinstance */,
              PWSTR /* pcmdline */,
              int /* ncmdshow */) -> int {
    auto window_manager { pane::window_manager() };

    auto webview { pane::webview(
        { u8"webview",
          pane::color { 0, 0, 0, 0 },
          pane::color { 255, 255, 255, 0 },
          true,
          nullptr },
        { .home_page = u8"about:blank",
          .creation_callback =
              [](pane::webview* webview) {
        webview->navigate(std::u8string(u8"file:///D:/mthierman/pane/pane/data/index.html"));

        pane::debug("OK!");
    } },
        [&](pane::webview* webview, pane::window_message window_message) -> LRESULT {
        switch (window_message.event) {
            case WM_CREATE: {
                window_manager.insert(webview->window_handle);
            } break;
            case WM_DESTROY: {
                window_manager.erase(webview->window_handle);
            } break;
        }

        return webview->default_procedure(window_message);
    }) };

    return pane::system::message_loop();
}
