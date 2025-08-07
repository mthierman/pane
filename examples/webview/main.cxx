#include <pane/pane.hxx>

struct webview : pane::webview<webview> {
    using Self = webview;
    using pane::webview<webview>::webview;

    static auto make_window_config() -> pane::window_config {
        return {
            u8"webview", pane::color { 0, 0, 0, 255 }, pane::color { 0, 0, 0, 255 }, true, nullptr
        };
    }

    static auto make_webview_config() -> pane::webview_config {
        return { u8"https://www.google.com/" };
    }

    auto handle_message(this Self& self, const pane::window_message& window_message) -> LRESULT {
        switch (window_message.msg) {
            using enum pane::webview_messages;

            case WM_DESTROY: {
                pane::system::quit();

                return 0;
            } break;

            case WM_KEYDOWN: {
                switch (window_message.wparam) {
                    case 'N': {
                        if (pane::input::is_key_down(VK_LCONTROL)
                            || pane::input::is_key_down(VK_RCONTROL)) {
                            // window_manager.create();
                        }
                    } break;
                    case 'W': {
                        if (pane::input::is_key_down(VK_LCONTROL)
                            || pane::input::is_key_down(VK_RCONTROL)) {
                            SendMessageW(window_message.hwnd, WM_CLOSE, 0, 0);
                        }
                    } break;
                }
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

// https://learn.microsoft.com/en-us/windows/win32/learnwin32/winmain--the-application-entry-point
auto wWinMain(HINSTANCE /* hinstance */,
              HINSTANCE /* hprevinstance */,
              PWSTR /* pcmdline */,
              int /* ncmdshow */) -> int {
    webview webview;

    return pane::system::message_loop();
}
