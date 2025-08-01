#include <pane/pane.hxx>

// https://learn.microsoft.com/en-us/windows/win32/learnwin32/winmain--the-application-entry-point
auto wWinMain(HINSTANCE /* hinstance */,
              HINSTANCE /* hprevinstance */,
              PWSTR /* pcmdline */,
              int /* ncmdshow */) -> int {
    pane::window_manager<pane::webview> window_manager {
        { u8"webview",
          pane::color { 0, 0, 0, 255 },
          pane::color { 255, 255, 255, 255 },
          true,
          nullptr },
        { u8"about:blank" },
        [&](const pane::window_message& window_message, pane::webview& webview) -> LRESULT {
        switch (window_message.msg) {
            case WM_CLOSE: {
                window_manager.remove(webview.window_handle());
            } break;

            case WM_KEYDOWN: {
                switch (window_message.wparam) {
                    case 'N': {
                        if (pane::input::is_key_down(VK_LCONTROL)
                            || pane::input::is_key_down(VK_RCONTROL)) {
                            window_manager.add(window_manager.window_config,
                                               window_manager.webview_config,
                                               window_manager.window_procedure);
                        }
                    } break;
                    case 'W': {
                        if (pane::input::is_key_down(VK_LCONTROL)
                            || pane::input::is_key_down(VK_RCONTROL)) {
                            SendMessageW(webview.window_handle(), WM_CLOSE, 0, 0);
                        }
                    } break;
                }
            } break;
        }

        return webview.default_procedure(window_message);
    }
    };

    return pane::system::message_loop();
}
