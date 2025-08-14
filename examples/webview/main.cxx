#include <pane/pane.hxx>

enum struct webview_message_type { init, test };

struct webview_message_payload {
    struct init {
        std::u8string name;
        uint64_t age { 0 };
    };
    struct test {
        uint64_t one { 0 };
        uint64_t two { 0 };
    };
};

template <> struct glz::meta<webview_message_type> {
    using enum webview_message_type;
    static constexpr auto value = glz::enumerate(init, test);
};

template <typename T = glz::json_t> struct webview_message {
    using Self = webview_message;

    webview_message_type type;
    T payload;
};

auto peek_type(const std::u8string& message) -> webview_message_type {
    webview_message webview_message;
    [[maybe_unused]] auto ec { glz::read_json(webview_message, message) };

    return webview_message.type;
}

struct webview : pane::webview<webview> {
    using Self = webview;
    using pane::webview<webview>::webview;

    static auto config() -> config {
        pane::window_config window_config {
            u8"webview", pane::color { 0, 0, 0, 255 }, pane::color { 0, 0, 0, 255 }, true, nullptr
        };

        pane::webview_config webview_config;

        if constexpr (pane::debug_mode) {
            return { window_config, pane::webview_config { u8"http://localhost:5173/" } };
        } else {
            return { window_config,
                     pane::webview_config {
                         u8"https://pane.internal/index.html",
                         pane::virtual_host { u8"pane.internal",
                                              u8"D:/mthierman/pane/examples/webview/gui/dist" } } };
        }
    }

    auto handle_message(this Self& self, const pane::window_message& window_message) -> LRESULT {
        switch (window_message.msg) {
            using enum pane::webview_messages;

            case +favicon_changed: {
                self.window_handle.icon(self.favicon());
            } break;

            case +core_created: {
                if (pane::debug_mode) {
                    self.devtools();
                }
            } break;

            case +navigation_completed: {
                self.window_handle.title(self.current_title);
                webview_message<webview_message_payload::init> data { webview_message_type::init,
                                                                      { u8"Abby Simpson", 18 } };
                self.post_json(data);
            } break;

            case +web_message_received: {
                switch (peek_type(self.current_message)) {
                    using enum webview_message_type;

                    case init: {
                        webview_message<webview_message_payload::init> asdasd;
                    } break;

                    case test: {
                    } break;
                }

                self.window_handle.title(self.current_message);
            } break;
        }

        return self.default_procedure(window_message);
    }
};

// https://learn.microsoft.com/en-us/windows/win32/learnwin32/winmain--the-application-entry-point
auto wWinMain(HINSTANCE /* hinstance */,
              HINSTANCE /* hprevinstance */,
              PWSTR /* pcmdline */,
              int /* ncmdshow */) -> int {
    webview webview;

    return pane::system::message_loop();
}
