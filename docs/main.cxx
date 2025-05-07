#include <pane/pane.hxx>

auto wWinMain(HINSTANCE /* hinstance */,
              HINSTANCE /* hprevinstance */,
              PWSTR /* pcmdline */,
              int /* ncmdshow */) -> int {
    auto gdi_plus { pane::gdi_plus() };

    EventRegistrationToken source_changed_token;
    EventRegistrationToken favicon_changed_token;
    wil::unique_hicon favicon;
    // HICON favicon;

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
                [webview](ICoreWebView2* /* sender */,
                          ICoreWebView2NavigationCompletedEventArgs* /* args */) -> HRESULT {
            wil::unique_cotaskmem_string title;
            webview->core->get_DocumentTitle(&title);
            SetWindowTextW(webview->window_handle(), title.get());

            return S_OK;
        }).Get(),
            &source_changed_token);

        webview->core->add_FaviconChanged(
            Microsoft::WRL::Callback<ICoreWebView2FaviconChangedEventHandler>(
                [webview, &favicon](ICoreWebView2* /* sender */, IUnknown* /* args */) -> HRESULT {
            wil::unique_cotaskmem_string favicon_uri;
            webview->core->get_FaviconUri(&favicon_uri);
            pane::debug(favicon_uri.get());

            webview->core->GetFavicon(
                COREWEBVIEW2_FAVICON_IMAGE_FORMAT::COREWEBVIEW2_FAVICON_IMAGE_FORMAT_PNG,
                Microsoft::WRL::Callback<ICoreWebView2GetFaviconCompletedHandler>(
                    [webview, &favicon](HRESULT error_code, IStream* icon_stream) -> HRESULT {
                // auto gdi_plus { pane::gdi_plus() };
                Gdiplus::Bitmap icon_bitmap(icon_stream);
                // HICON favicon;
                // wil::unique_hicon favicon;

                if (icon_bitmap.GetHICON(&favicon) == Gdiplus::Status::Ok) {
                    pane::debug("OK!");

                    SendMessage(
                        webview->window_handle(), WM_SETICON, ICON_SMALL, LPARAM(favicon.get()));

                    SendMessage(
                        webview->window_handle(), WM_SETICON, ICON_BIG, LPARAM(favicon.get()));
                } else {
                    pane::debug("Not OK!");
                }

                return S_OK;
            }).Get());

            return S_OK;
        }).Get(),
            &favicon_changed_token);
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
