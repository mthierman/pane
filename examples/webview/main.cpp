#include <pane/pane.hpp>

enum struct event_type { init, test };

template <> struct glz::meta<event_type> {
    using enum event_type;
    static constexpr auto value = glz::enumerate(init, test);
};

struct event_payload {
    struct init {
        std::u8string url;
    };
    struct test {
        uint64_t one {};
        uint64_t two {};
    };
};

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

            case +created: {
                if (pane::debug_mode) {
                    self.devtools();
                }
            } break;

            case +navigation_completed: {
                self.window_handle.title(self.current_title);

                self.post_event<pane::webview_event<event_type, event_payload::init>>(
                    { event_type::init, { self.webview_config.home_page } });
            } break;

            case +web_message_received: {
                // switch (self.peek_event<event_type>(self.current_message)) {
                //     using enum event_type;

                //     case init: {
                //         auto event { self.make_event<event_type, event_payload::init>(
                //             init, self.current_message) };

                //         pane::debug("init_message.payload.name: {}", event.payload.name);
                //     } break;

                //     case test: {
                //         auto event { self.make_event<event_type, event_payload::test>(
                //             test, self.current_message) };

                //         pane::debug("init_message.payload.name: {}", event.payload.one);
                //     } break;
                // }
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
