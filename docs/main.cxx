#include <pane/pane.hxx>

auto wWinMain(HINSTANCE /* hinstance */,
              HINSTANCE /* hprevinstance */,
              PWSTR /* pcmdline */,
              int /* ncmdshow */) -> int {
    EventRegistrationToken source_changed_token;

    auto url { pane::webview(
        { u8"WebView2",
          pane::color { 0, 0, 0, 0 },
          pane::color { 255, 255, 255, 0 },
          true,
          true,
          nullptr },
        { .home_page = u8"https://learn.microsoft.com/en-us/microsoft-edge/webview2/concepts/"
                       u8"working-with-local-content?tabs=dotnetcsharp",
          .creation_callback =
              [&](pane::webview* webview) {
        webview->core->add_NavigationCompleted(
            Microsoft::WRL::Callback<ICoreWebView2NavigationCompletedEventHandler>(
                [webview](ICoreWebView2* /* webview */,
                          ICoreWebView2NavigationCompletedEventArgs* /* args */) {
            wil::unique_cotaskmem_string title;
            webview->core->get_DocumentTitle(&title);
            SetWindowTextW(webview->window_handle(), title.get());

            return S_OK;
        }).Get(),
            &source_changed_token);
    } },
        [&](pane::webview* webview, pane::window_message window_message) -> LRESULT {
        switch (window_message.event) {
            case WM_DESTROY: {
                PostQuitMessage(0);

                return 0;
            } break;

            case WM_SETTINGCHANGE: {
                webview->core->Reload();
            } break;
        }

        return webview->default_procedure(window_message);
    }) };

    return pane::system::message_loop();
}
