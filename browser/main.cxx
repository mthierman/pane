#include <pane/pane.hxx>
#include <optional>

using Microsoft::WRL::Callback;

auto wWinMain(HINSTANCE /* hinstance */,
              HINSTANCE /* hprevinstance */,
              PWSTR /* pcmdline */,
              int /* ncmdshow */) -> int {
    auto args { pane::system::command_line_arguments() };

    auto home_page { std::optional<std::u8string> { std::nullopt } };

    if (args.size() == 2) {
        auto arg { args.at(1) };
        if (arg.starts_with(u8"https://")) {
            home_page = args.at(1);
        } else {
            home_page = args.at(1).insert(0, u8"https://");
        }
    }

    auto gdi_plus { pane::gdi_plus() };

    struct event_token {
        pane::webview_token source_changed;
        pane::webview_token favicon_changed;
        pane::webview_token accelerator_key_pressed;
    };

    event_token token;
    pane::window_icon favicon;

    auto browser { pane::webview(
        { u8"Browser",
          pane::color { 0, 0, 0, 255 },
          pane::color { 255, 255, 255, 255 },
          true,
          nullptr },
        { .home_page = home_page ? *home_page : u8"about:blank",
          .creation_callback = [&](pane::webview* webview) -> void {
        webview->core->add_NavigationCompleted(
            Callback<ICoreWebView2NavigationCompletedEventHandler>(
                [&, webview](ICoreWebView2* /* sender */,
                             ICoreWebView2NavigationCompletedEventArgs* /* args */) -> HRESULT {
            wil::unique_cotaskmem_string title;
            webview->core->get_DocumentTitle(&title);
            SetWindowTextW(webview->window_handle(), title.get());

            return S_OK;
        }).Get(),
            token.source_changed());

        webview->core->add_FaviconChanged(
            Callback<ICoreWebView2FaviconChangedEventHandler>(
                [&, webview](ICoreWebView2* /* sender */, IUnknown* /* args */) -> HRESULT {
            webview->core->GetFavicon(
                COREWEBVIEW2_FAVICON_IMAGE_FORMAT::COREWEBVIEW2_FAVICON_IMAGE_FORMAT_PNG,
                Callback<ICoreWebView2GetFaviconCompletedHandler>(
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

        // webview->controller->add_AcceleratorKeyPressed(
        //     Callback<ICoreWebView2AcceleratorKeyPressedEventHandler>(
        //         [&](ICoreWebView2Controller* sender,
        //             ICoreWebView2AcceleratorKeyPressedEventArgs* args) -> HRESULT {
        //     //
        //     return S_OK;
        // }).Get(),
        //     token.accelerator_key_pressed());
    } },
        [&](pane::webview* webview, pane::window_message window_message) -> LRESULT {
        switch (window_message.event) {
            case WM_CREATE: {
                webview->window_handle.maximize();

                return 0;
            } break;

            case WM_DESTROY: {
                pane::system::quit();

                return 0;
            } break;
        }

        return webview->default_procedure(window_message);
    }) };

    return pane::system::message_loop();
}
