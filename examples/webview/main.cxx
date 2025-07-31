#include <pane/pane.hxx>

// https://learn.microsoft.com/en-us/windows/win32/learnwin32/winmain--the-application-entry-point
auto wWinMain(HINSTANCE /* hinstance */,
              HINSTANCE /* hprevinstance */,
              PWSTR /* pcmdline */,
              int /* ncmdshow */) -> int {
    pane::window_manager window_manager;

    auto url { pane::webview {
        { u8"url", pane::color { 0, 0, 0, 0 }, pane::color { 255, 255, 255, 0 }, true, nullptr },
        { u8"about:blank" },
        [&](const pane::window_message& window_message, pane::webview* webview) -> LRESULT {
        auto& self = *webview;

        switch (window_message.msg) {
            using enum pane::webview::message;

            case WM_CREATE: {
                window_manager.insert(self.window_handle);
            } break;

            case WM_DESTROY: {
                window_manager.erase(self.window_handle);
            } break;

            case +WEBVIEW_CREATE: {
                self.navigate(u8"https://learn.microsoft.com/windows/apps/winui/winui3/");
            } break;
        }

        return self.default_procedure(window_message);
    } } };

    auto virtual_host { pane::webview {
        { u8"virtual_host",
          pane::color { 0, 0, 0, 0 },
          pane::color { 255, 255, 255, 0 },
          true,
          nullptr },
        { u8"about:blank",
          std::make_pair(u8"pane.internal",
                         std::filesystem::path(u8"D:/mthierman/pane/pane/data")) },
        [&](const pane::window_message& window_message, pane::webview* webview) -> LRESULT {
        auto& self = *webview;

        switch (window_message.msg) {
            using enum pane::webview::message;

            case WM_CREATE: {
                window_manager.insert(self.window_handle);
            } break;

            case WM_DESTROY: {
                window_manager.erase(self.window_handle);
            } break;

            case +WEBVIEW_CREATE: {
                self.navigate(u8"https://pane.internal/index.html");
            } break;
        }

        return self.default_procedure(window_message);
    } } };

    auto file { pane::webview {
        { u8"file", pane::color { 0, 0, 0, 0 }, pane::color { 255, 255, 255, 0 }, true, nullptr },
        { u8"about:blank" },
        [&](const pane::window_message& window_message, pane::webview* webview) -> LRESULT {
        auto& self = *webview;

        switch (window_message.msg) {
            using enum pane::webview::message;

            case WM_CREATE: {
                window_manager.insert(self.window_handle);
            } break;

            case WM_DESTROY: {
                window_manager.erase(self.window_handle);
            } break;

            case +WEBVIEW_CREATE: {
                self.navigate(std::u8string(u8"file:///D:/mthierman/pane/pane/data/index.html"));
            } break;
        }

        return self.default_procedure(window_message);
    } } };

    auto navigate_to_string { pane::webview {
        { u8"navigate_to_string",
          pane::color { 0, 0, 0, 0 },
          pane::color { 255, 255, 255, 0 },
          true,
          nullptr },
        { u8"about:blank",
          std::make_pair(u8"pane.internal",
                         std::filesystem::path(u8"D:/mthierman/pane/pane/data")) },
        [&](const pane::window_message& window_message, pane::webview* webview) -> LRESULT {
        auto& self = *webview;

        switch (window_message.msg) {
            using enum pane::webview::message;

            case WM_CREATE: {
                window_manager.insert(self.window_handle);
            } break;

            case WM_DESTROY: {
                window_manager.erase(self.window_handle);
            } break;

            case +WEBVIEW_CREATE: {
                self.navigate_to_string(
                    u8R"(<html><body style="background-color: black; color: white;">navigate_to_string</body></html>)");
            } break;
        }

        return self.default_procedure(window_message);
    } } };

    return pane::system::message_loop();
}
