#include <pane/pane.hxx>

auto wWinMain(HINSTANCE /* hinstance */,
              HINSTANCE /* hprevinstance */,
              PWSTR /* pcmdline */,
              int /* ncmdshow */) -> int {
    auto gdi_plus { pane::gdi_plus() };

    struct event_token {
        pane::webview_token source_changed;
        pane::webview_token favicon_changed;
    };

    event_token token;
    pane::window_icon favicon;

    auto webview2_docs { pane::webview(
        { u8"WebView2",
          pane::color { 0, 0, 0, 0 },
          pane::color { 255, 255, 255, 0 },
          true,
          true,
          nullptr },
        { .home_page = u8"https://learn.microsoft.com/en-us/microsoft-edge/webview2/concepts/"
                       u8"win32-api-conventions",
          .creation_callback = [&](pane::webview* webview) -> void {
        webview->core->add_NavigationCompleted(
            Microsoft::WRL::Callback<ICoreWebView2NavigationCompletedEventHandler>(
                [&, webview](ICoreWebView2* /* sender */,
                             ICoreWebView2NavigationCompletedEventArgs* /* args */) -> HRESULT {
            wil::unique_cotaskmem_string title;
            webview->core->get_DocumentTitle(&title);
            SetWindowTextW(webview->window_handle(), title.get());

            return S_OK;
        }).Get(),
            token.source_changed());

        webview->core->add_FaviconChanged(
            Microsoft::WRL::Callback<ICoreWebView2FaviconChangedEventHandler>(
                [&, webview](ICoreWebView2* /* sender */, IUnknown* /* args */) -> HRESULT {
            webview->core->GetFavicon(
                COREWEBVIEW2_FAVICON_IMAGE_FORMAT::COREWEBVIEW2_FAVICON_IMAGE_FORMAT_PNG,
                Microsoft::WRL::Callback<ICoreWebView2GetFaviconCompletedHandler>(
                    [&, webview](HRESULT /* error_code */, IStream* icon_stream) -> HRESULT {
                if (Gdiplus::Bitmap { icon_stream }.GetHICON(&favicon()) == Gdiplus::Status::Ok) {
                    SendMessage(
                        webview->window_handle(), WM_SETICON, ICON_SMALL, LPARAM(favicon()));
                    SendMessage(webview->window_handle(), WM_SETICON, ICON_BIG, LPARAM(favicon()));
                }

                return S_OK;
            }).Get());

            return S_OK;
        }).Get(),
            token.favicon_changed());
    } },
        [&](pane::webview* webview, pane::window_message window_message) -> LRESULT {
        switch (window_message.event) {
            case WM_DESTROY: {
                pane::system::quit();

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
