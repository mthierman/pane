#include <pane/pane.hxx>

enum struct event_type { init, test };

struct event_payload {
    struct init {
        std::u8string name;
        uint64_t age { 0 };
    };
    struct test {
        uint64_t one { 0 };
        uint64_t two { 0 };
    };
};

template <> struct glz::meta<event_type> {
    using enum event_type;
    static constexpr auto value = glz::enumerate(init, test);
};

template <typename T = glz::json_t> struct event {
    using Self = event;

    event_type type;
    T payload;
};

auto peek_event_type(std::u8string_view message) -> event_type {
    event event;
    [[maybe_unused]] auto ec { glz::read_json(event, message) };

    return event.type;
}

template <typename T>
auto make_webview_event(event_type type, std::u8string_view message) -> event<T> {
    event<T> event;
    event.type = type;
    [[maybe_unused]] auto ec { glz::read_json(event, message) };

    return event;
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

                self.post_event<event<event_payload::init>>(
                    { event_type::init, { u8"Abby Simpson", 18 } });
            } break;

            case +web_message_received: {
                switch (peek_event_type(self.current_message)) {
                    using enum event_type;

                    case init: {
                        auto event { make_webview_event<event_payload::init>(
                            init, self.current_message) };

                        pane::debug("init_message.payload.name: {}", event.payload.name);
                    } break;

                    case test: {
                        auto event { make_webview_event<event_payload::test>(
                            test, self.current_message) };

                        pane::debug("init_message.payload.name: {}", event.payload.one);
                    } break;
                }
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
