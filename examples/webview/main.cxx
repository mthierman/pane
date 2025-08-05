#include <pane/pane.hxx>

struct webview : pane::webview<webview> {
    using Self = webview;
    using pane::webview<webview>::webview;

    auto message_handler(this Self& self, const pane::window_message& window_message) -> LRESULT {
        switch (window_message.msg) {
            using enum webview::message;

            case WM_CLOSE: {
                pane::system::quit();
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

            case +NAVIGATION_COMPLETED: {
                self.window_handle.title(self.current_title);
            } break;

            case +FAVICON_CHANGED: {
                self.window_handle.icon(self.favicon());
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
    webview window { { u8"webview",
                       pane::color { 0, 0, 0, 255 },
                       pane::color { 255, 255, 255, 255 },
                       true,
                       nullptr },
                     { u8"https://www.google.com/" } };

    return pane::system::message_loop();
}
