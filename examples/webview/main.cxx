#include <pane/pane.hxx>

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

            case +navigation_completed: {
                self.window_handle.title(self.current_title);
                self.post_json(u8R"({
    "type": "greeting",
    "message": "Hello from native C++!",
    "timestamp": 1723058842,
    "data": {
        "id": 123,
        "active": true
    }
})");
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
