#include <pane/pane.hxx>

// https://learn.microsoft.com/en-us/windows/win32/learnwin32/winmain--the-application-entry-point
auto wWinMain(HINSTANCE /* hinstance */,
              HINSTANCE /* hprevinstance */,
              PWSTR /* pcmdline */,
              int /* ncmdshow */) -> int {
    auto window_manager { pane::window_manager() };

    auto url { pane::webview(
        { u8"url", pane::color { 0, 0, 0, 0 }, pane::color { 255, 255, 255, 0 }, true, nullptr },
        { .home_page = u8"about:blank",
          .creation_callback
          = [](pane::webview* webview) { webview->navigate(u8"https://www.google.com/"); } },
        [&](pane::webview* webview, pane::window_message window_message) -> LRESULT {
        switch (window_message.event) {
            case WM_CREATE: {
                window_manager.insert(webview->window_handle);
            } break;
            case WM_DESTROY: {
                window_manager.erase(webview->window_handle);
            } break;
        }

        return webview->default_procedure(window_message);
    }) };

    auto virtual_host { pane::webview(
        { u8"virtual_host",
          pane::color { 0, 0, 0, 0 },
          pane::color { 255, 255, 255, 0 },
          true,
          nullptr },
        { .home_page = u8"about:blank",
          .virtual_host_name_map = std::make_pair(
              u8"pane.internal", std::filesystem::path(u8"D:/mthierman/pane/pane/data")),
          .creation_callback =
              [](pane::webview* webview) {
        webview->navigate(u8"https://pane.internal/index.html");
    } },
        [&](pane::webview* webview, pane::window_message window_message) -> LRESULT {
        switch (window_message.event) {
            case WM_CREATE: {
                window_manager.insert(webview->window_handle);
            } break;
            case WM_DESTROY: {
                window_manager.erase(webview->window_handle);
            } break;
        }

        return webview->default_procedure(window_message);
    }) };

    auto file { pane::webview(
        pane::window_config {
            u8"file", pane::color { 0, 0, 0, 0 }, pane::color { 255, 255, 255, 0 }, true, nullptr },
        pane::webview_config { .home_page = u8"about:blank",
                               .creation_callback =
                                   [](pane::webview* webview) {
        webview->navigate(std::u8string(u8"file:///D:/mthierman/pane/pane/data/index.html"));
    } },
        [&](pane::webview* webview, pane::window_message window_message) -> LRESULT {
        switch (window_message.event) {
            case WM_CREATE: {
                window_manager.insert(webview->window_handle);
            } break;
            case WM_DESTROY: {
                window_manager.erase(webview->window_handle);
            } break;
        }

        return webview->default_procedure(window_message);
    }) };

    auto navigate_to_string { pane::webview(
        pane::window_config { u8"navigate_to_string",
                              pane::color { 0, 0, 0, 0 },
                              pane::color { 255, 255, 255, 0 },
                              true,
                              nullptr },
        pane::webview_config {
            .home_page = u8"about:blank",
            .virtual_host_name_map = std::make_pair(
                u8"pane.internal", std::filesystem::path(u8"D:/mthierman/pane/pane/data")),
            .creation_callback =
                [](pane::webview* webview) {
        webview->navigate_to_string(u8R"(<html><body>navigate_to_string</body></html>)");
    } },
        [&](pane::webview* webview, pane::window_message window_message) -> LRESULT {
        switch (window_message.event) {
            case WM_CREATE: {
                window_manager.insert(webview->window_handle);
            } break;
            case WM_DESTROY: {
                window_manager.erase(webview->window_handle);
            } break;
        }

        return webview->default_procedure(window_message);
    }) };

    return pane::system::message_loop();
}
