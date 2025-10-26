#include <pane/pane.hpp>
#include <optional>

struct webview : pane::webview<webview> {
    using Self = webview;
    using pane::webview<webview>::webview;

    static auto config() -> config {
        auto args { pane::system::get_argv_u8() };
        std::optional<std::u8string> home_page { std::nullopt };

        // if (args.size() == 2) {
        //     if (args.at(1).starts_with(u8"https://")) {
        //         home_page = args.at(1);
        //     } else {
        //         home_page = args.at(1).insert(0, u8"https://");
        //     }
        // }

        std::filesystem::path browser_data;
        auto local_app_data { pane::filesystem::known_folder() };

        if (local_app_data) {
            browser_data = *local_app_data / u8"browser";
        }

        return { pane::window_config { u8"webview",
                                       pane::color { 0, 0, 0, 255 },
                                       pane::color { 0, 0, 0, 255 },
                                       true,
                                       nullptr },
                 pane::webview_config { home_page.value_or(u8"http://localhost:4321/"),
                                        std::nullopt,
                                        u8"",
                                        browser_data } };
    }

    auto handle_message(this Self& self, const pane::window_message& window_message) -> LRESULT {
        switch (window_message.msg) {
            using enum pane::webview_messages;

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
    pane::window_manager<webview> browser;

    return pane::system::message_loop();
}
