#include <pane/pane.hxx>
#include <optional>

struct webview : pane::webview<webview> {
    using Self = webview;
    using pane::webview<webview>::webview;

    static auto config() -> config {
        auto args { pane::system::command_line_arguments() };
        std::optional<std::u8string> home_page { std::nullopt };

        if (args.size() == 2) {
            if (args.at(1).starts_with(u8"https://")) {
                home_page = args.at(1);
            } else {
                home_page = args.at(1).insert(0, u8"https://");
            }
        }

        return { { u8"webview",
                   pane::color { 0, 0, 0, 255 },
                   pane::color { 0, 0, 0, 255 },
                   true,
                   nullptr },
                 { home_page.value_or(u8"https://www.google.com/") } };
    }

    auto handle_message(this Self& self, const pane::window_message& window_message) -> LRESULT {
        switch (window_message.msg) {
            using enum pane::webview_messages;

            case WM_DESTROY: {
                pane::system::quit();

                return 0;
            } break;

            case +favicon_changed: {
                self.window_handle.icon(self.favicon());
            } break;

            case +navigation_completed: {
                self.window_handle.title(self.current_title);
            } break;
        }

        return self.default_procedure(window_message);
    }
};

auto wWinMain(HINSTANCE /* hinstance */,
              HINSTANCE /* hprevinstance */,
              PWSTR /* pcmdline */,
              int /* ncmdshow */) -> int {
    webview browser;

    return pane::system::message_loop();
}
