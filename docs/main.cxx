#include <pane/pane.hxx>

auto wWinMain(HINSTANCE /* hinstance */,
              HINSTANCE /* hprevinstance */,
              PWSTR /* pcmdline */,
              int /* ncmdshow */) -> int {
    auto url { pane::webview(
        { u8"WebView2",
          pane::color { 0, 0, 0, 0 },
          pane::color { 255, 255, 255, 0 },
          true,
          nullptr },
        { .home_page = u8"https://learn.microsoft.com/en-us/microsoft-edge/webview2/concepts/"
                       u8"working-with-local-content?tabs=dotnetcsharp" },
        [&](pane::webview* webview, pane::window_message window_message) -> LRESULT {
        switch (window_message.event) {
            case WM_DESTROY: {
                PostQuitMessage(0);

                return 0;
            }
        }

        return webview->default_procedure(window_message);
    }) };

    return pane::system::message_loop();
}
