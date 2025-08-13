#include <pane/pane.hxx>

template <typename T> auto to_json(const T& value) -> std::u8string {
    std::u8string buffer;

    [[maybe_unused]] auto ec { glz::write<glz::opts { .prettify { true } }>(value, buffer) };

    return buffer;
}

template <typename T> auto to_schema(const T& value) -> std::u8string {
    std::u8string buffer;

    [[maybe_unused]] auto ec { glz::write_json_schema<T>(buffer) };

    return buffer;
}

struct init_data {
    using Self = init_data;

    struct payload {
        std::u8string name;
        uint64_t age { 0 };
    };

    std::u8string type { u8"init_data" };
    payload payload { u8"Abbie Mays", 18 };
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
            return { window_config, pane::webview_config { u8"https://localhost:5173/" } };
        } else {
            return { window_config,
                     pane::webview_config {
                         u8"https://pane.internal/index.html",
                         pane::virtual_host { u8"pane.internal", u8"D:/mthierman/pane/data" } } };
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
                init_data data;
                self.post_json(data);
            } break;

            case +web_message_received: {
                glz::json_t json;
                [[maybe_unused]] auto ec { glz::read_json(json, self.current_message) };

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
