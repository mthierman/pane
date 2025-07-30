#include <pane/pane.hxx>
#include <dwmapi.h>

using Microsoft::WRL::Callback;

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

    pane::webview webview2_docs { pane::webview(
        { u8"WebView2",
          pane::color { 0, 0, 0, 255 },
          pane::color { 255, 255, 255, 255 },
          true,
          nullptr },
        { .home_page = u8"https://learn.microsoft.com/en-us/microsoft-edge/webview2/reference/"
                       u8"win32/" },
        [&](pane::window_message window_message) -> LRESULT {
        switch (window_message.event) {
            using enum pane::webview::message;

            case WM_DESTROY: {
                pane::system::quit();

                return 0;
            } break;

            case WM_SETTINGCHANGE: {
                // if (webview2_docs.core) {
                //     webview2_docs.core->Reload();
                // }
            } break;

            case std::to_underlying(WEBVIEW_CREATE): {
                webview2_docs.core->add_NavigationCompleted(
                    Callback<ICoreWebView2NavigationCompletedEventHandler>(
                        [&](ICoreWebView2* /* sender */,
                            ICoreWebView2NavigationCompletedEventArgs* /* args */) -> HRESULT {
                    wil::unique_cotaskmem_string title;
                    webview2_docs.core->get_DocumentTitle(&title);
                    SetWindowTextW(webview2_docs.window_handle(), title.get());

                    // webview->core->ExecuteScript(
                    //     LR"(document.documentElement.style.background = "#00000000";
                    //     document.body.style.background = "#00000000")",
                    //     Callback<ICoreWebView2ExecuteScriptCompletedHandler>(
                    //         [](HRESULT /* error_code */, PCWSTR result) -> HRESULT {
                    //     return S_OK;
                    // }).Get());

                    return S_OK;
                }).Get(),
                    token.source_changed());

                webview2_docs.core->add_FaviconChanged(
                    Callback<ICoreWebView2FaviconChangedEventHandler>(
                        [&](ICoreWebView2* /* sender */, IUnknown* /* args */) -> HRESULT {
                    webview2_docs.core->GetFavicon(
                        COREWEBVIEW2_FAVICON_IMAGE_FORMAT::COREWEBVIEW2_FAVICON_IMAGE_FORMAT_PNG,
                        Callback<ICoreWebView2GetFaviconCompletedHandler>(
                            [&](HRESULT /* error_code */, IStream* icon_stream) -> HRESULT {
                        if (Gdiplus::Bitmap { icon_stream }.GetHICON(&favicon())
                            == Gdiplus::Status::Ok) {
                            SendMessage(webview2_docs.window_handle(),
                                        WM_SETICON,
                                        ICON_SMALL,
                                        LPARAM(favicon()));
                            SendMessage(webview2_docs.window_handle(),
                                        WM_SETICON,
                                        ICON_BIG,
                                        LPARAM(favicon()));
                        }

                        return S_OK;
                    }).Get());

                    return S_OK;
                }).Get(),
                    token.favicon_changed());
            } break;
        }

        return webview2_docs.default_procedure(window_message);
    }) };

    return pane::system::message_loop();
}
