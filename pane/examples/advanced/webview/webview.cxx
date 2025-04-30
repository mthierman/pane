#include <pane/pane.hxx>

auto wWinMain(HINSTANCE /* hinstance */,
              HINSTANCE /* hprevinstance */,
              PWSTR /* pcmdline */,
              int /* ncmdshow */) -> int {
    auto window_manager { pane::window_manager() };
    auto bg { pane::color { 0, 0, 0, 255 } };

    auto webview { pane::webview(
        { u8"webview", bg, true, nullptr },
        { .home_page = u8"https://www.google.com/" },
        [&](pane::webview* webview, pane::window_message window_message) -> LRESULT {
        switch (window_message.event) {
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
