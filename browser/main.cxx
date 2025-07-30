#include <pane/pane.hxx>
#include <optional>

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

    struct event_token {
        pane::webview_token source_changed;
        pane::webview_token favicon_changed;
    };

    event_token token;

    pane::webview browser { { u8"Browser",
                              pane::color { 0, 0, 0, 255 },
                              pane::color { 255, 255, 255, 255 },
                              true,
                              nullptr },
                            { home_page.value_or(u8"about:blank") },
                            [&](pane::window_message window_message) -> LRESULT {
        switch (window_message.event) {
            using enum pane::webview::message;

            case WM_CREATE: {
                // browser.window_handle.maximize();

                return 0;
            } break;

            case WM_DESTROY: {
                pane::system::quit();

                return 0;
            } break;

            case std::to_underlying(WEBVIEW_CREATE): {
                browser.core->add_FaviconChanged(
                    Microsoft::WRL::Callback<ICoreWebView2FaviconChangedEventHandler>(
                        [&](ICoreWebView2* /* sender */, IUnknown* /* args */) -> HRESULT {
                    browser.core->GetFavicon(
                        COREWEBVIEW2_FAVICON_IMAGE_FORMAT::COREWEBVIEW2_FAVICON_IMAGE_FORMAT_PNG,
                        Microsoft::WRL::Callback<ICoreWebView2GetFaviconCompletedHandler>(
                            [&](HRESULT /* error_code */, IStream* icon_stream) -> HRESULT {
                        if (Gdiplus::Bitmap { icon_stream }.GetHICON(&browser.favicon())
                            == Gdiplus::Status::Ok) {
                            SendMessage(browser.window_handle(),
                                        WM_SETICON,
                                        ICON_SMALL,
                                        reinterpret_cast<LPARAM>(browser.favicon()));
                            SendMessage(browser.window_handle(),
                                        WM_SETICON,
                                        ICON_BIG,
                                        reinterpret_cast<LPARAM>(browser.favicon()));
                        }

                        return S_OK;
                    }).Get());

                    return S_OK;
                }).Get(),
                    token.favicon_changed());

                browser.core->add_NavigationCompleted(
                    Microsoft::WRL::Callback<ICoreWebView2NavigationCompletedEventHandler>(
                        [&](ICoreWebView2* /* sender */,
                            ICoreWebView2NavigationCompletedEventArgs* /* args */) -> HRESULT {
                    wil::unique_cotaskmem_string title;
                    browser.core->get_DocumentTitle(&title);
                    SetWindowTextW(browser.window_handle(), title.get());

                    return S_OK;
                }).Get(),
                    token.source_changed());
            } break;
        }

        return browser.default_procedure(window_message);
    } };

    return pane::system::message_loop();
}
