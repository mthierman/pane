#include <pane/pane.hxx>

// https://learn.microsoft.com/en-us/windows/win32/learnwin32/winmain--the-application-entry-point
auto wWinMain(HINSTANCE /* hinstance */,
              HINSTANCE /* hprevinstance */,
              PWSTR /* pcmdline */,
              int /* ncmdshow */) -> int {
    pane::window_manager window_manager;

    pane::webview url {
        { u8"url", pane::color { 0, 0, 0, 0 }, pane::color { 255, 255, 255, 0 }, true, nullptr },
        { u8"about:blank" },
        [&](pane::window_message window_message) -> LRESULT {
        switch (window_message.event) {
            using enum pane::webview::message;

            case WM_CREATE: {
                window_manager.insert(url.window_handle);
            } break;

            case WM_DESTROY: {
                window_manager.erase(url.window_handle);
            } break;

            case std::to_underlying(WEBVIEW_CREATE): {
                url.navigate(u8"https://learn.microsoft.com/windows/apps/winui/winui3/");
            } break;
        }

        return url.default_procedure(window_message);
    }
    };

    pane::webview virtual_host { { u8"virtual_host",
                                   pane::color { 0, 0, 0, 0 },
                                   pane::color { 255, 255, 255, 0 },
                                   true,
                                   nullptr },
                                 { u8"about:blank",
                                   std::make_pair(
                                       u8"pane.internal",
                                       std::filesystem::path(u8"D:/mthierman/pane/pane/data")) },
                                 [&](pane::window_message window_message) -> LRESULT {
        switch (window_message.event) {
            using enum pane::webview::message;

            case WM_CREATE: {
                window_manager.insert(virtual_host.window_handle);
            } break;

            case WM_DESTROY: {
                window_manager.erase(virtual_host.window_handle);
            } break;

            case std::to_underlying(WEBVIEW_CREATE): {
                virtual_host.navigate(u8"https://pane.internal/index.html");
            } break;
        }

        return virtual_host.default_procedure(window_message);
    } };

    pane::webview file {
        { u8"file", pane::color { 0, 0, 0, 0 }, pane::color { 255, 255, 255, 0 }, true, nullptr },
        { u8"about:blank" },
        [&](pane::window_message window_message) -> LRESULT {
        switch (window_message.event) {
            using enum pane::webview::message;

            case WM_CREATE: {
                window_manager.insert(file.window_handle);
            } break;

            case WM_DESTROY: {
                window_manager.erase(file.window_handle);
            } break;

            case std::to_underlying(WEBVIEW_CREATE): {
                file.navigate(std::u8string(u8"file:///D:/mthierman/pane/pane/data/index.html"));
            } break;
        }

        return file.default_procedure(window_message);
    }
    };

    pane::webview navigate_to_string {
        { u8"navigate_to_string",
          pane::color { 0, 0, 0, 0 },
          pane::color { 255, 255, 255, 0 },
          true,
          nullptr },
        { u8"about:blank",
          std::make_pair(u8"pane.internal",
                         std::filesystem::path(u8"D:/mthierman/pane/pane/data")) },
        [&](pane::window_message window_message) -> LRESULT {
        switch (window_message.event) {
            using enum pane::webview::message;

            case WM_CREATE: {
                window_manager.insert(navigate_to_string.window_handle);
            } break;

            case WM_DESTROY: {
                window_manager.erase(navigate_to_string.window_handle);
            } break;

            case std::to_underlying(WEBVIEW_CREATE): {
                navigate_to_string.navigate_to_string(
                    u8R"(<html><body style="background-color: black; color: white;">navigate_to_string</body></html>)");
            } break;
        }

        return navigate_to_string.default_procedure(window_message);
    }
    };

    return pane::system::message_loop();
}
