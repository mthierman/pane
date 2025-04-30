#include <pane/pane.hxx>

auto wWinMain(HINSTANCE /* hinstance */,
              HINSTANCE /* hprevinstance */,
              PWSTR /* pcmdline */,
              int /* ncmdshow */) -> int {
    auto webview { pane::webview(
        { u8"webview", pane::color { 0, 0, 0, 255 }, true, nullptr },
        { .home_page = u8"about:blank" },
        [&](pane::webview* webview, pane::window_message window_message) -> LRESULT {
        switch (window_message.event) {
            case WM_CREATE: {
                pane::debug("WM_CREATE");
            } break;
            case WM_DESTROY: {
                PostQuitMessage(0);

                return 1;
            }
        }

        return window_message.default_procedure();
    }) };

    return pane::system::message_loop();
}
