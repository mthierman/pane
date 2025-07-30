#include <pane/pane.hxx>
#include <optional>

auto wWinMain(HINSTANCE /* hinstance */,
              HINSTANCE /* hprevinstance */,
              PWSTR /* pcmdline */,
              int /* ncmdshow */) -> int {
    auto args { pane::system::command_line_arguments() };
    std::optional<std::u8string> home_page { std::nullopt };

    if (args.size() == 2) {
        if (args.at(1).starts_with(u8"https://")) {
            home_page = args.at(1);
        } else {
            home_page = args.at(1).insert(0, u8"https://");
        }
    }

    pane::webview browser { { u8"Browser",
                              pane::color { 0, 0, 0, 255 },
                              pane::color { 255, 255, 255, 255 },
                              true,
                              nullptr },
                            { home_page.value_or(u8"about:blank") },
                            [&](pane::window_message window_message) -> LRESULT {
        switch (window_message.event) {
            using enum pane::webview::message;

            case WM_DESTROY: {
                pane::system::quit();

                return 0;
            } break;

            case std::to_underlying(FAVICON_CHANGED): {
                SendMessage(browser.window_handle(),
                            WM_SETICON,
                            ICON_SMALL,
                            reinterpret_cast<LPARAM>(browser.favicon()));
                SendMessage(browser.window_handle(),
                            WM_SETICON,
                            ICON_BIG,
                            reinterpret_cast<LPARAM>(browser.favicon()));
            } break;

            case std::to_underlying(NAVIGATION_COMPLETED): {
                // SetWindowTextW(
                //     browser.window_handle(),
                //     reinterpret_cast<const
                //     wchar_t*>(pane::to_utf16(browser.current_title).data()));
                browser.window_handle.title(browser.current_title);
            } break;
        }

        return browser.default_procedure(window_message);
    } };

    return pane::system::message_loop();
}
