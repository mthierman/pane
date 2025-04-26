#include <pane/pane.hxx>
#include <set>

namespace pane {
struct window_manager {
    std::set<HWND> windows;

    auto insert(HWND hwnd) -> void { windows.insert(hwnd); }

    auto erase(HWND hwnd) -> void {
        windows.erase(hwnd);

        if (windows.empty()) {
            PostQuitMessage(0);
        }
    }
};
}; // namespace pane

auto make_window(pane::window_manager& window_manager) -> pane::window {
    return pane::window({ u8"pane", pane::color { 0, 0, 0, 0 }, true, false },
                        [&](pane::window::message message) -> LRESULT {
        if (message.msg == WM_CREATE) {
            window_manager.insert(message.hwnd);
        }

        if (message.msg == WM_DESTROY) {
            window_manager.erase(message.hwnd);
        }

        return DefWindowProcW(message.hwnd, message.msg, message.wparam, message.lparam);
    });
}

auto make_webview(pane::window_manager& window_manager) -> pane::webview {
    return pane::webview({},
                         { .home_page = u8"https://www.google.com/" },
                         [&](pane::window::message message) -> LRESULT {
        if (message.msg == WM_CREATE) {
            window_manager.insert(message.hwnd);
        }

        if (message.msg == WM_DESTROY) {
            window_manager.erase(message.hwnd);
        }

        return DefWindowProcW(message.hwnd, message.msg, message.wparam, message.lparam);
    });
}

// https://learn.microsoft.com/en-us/windows/win32/learnwin32/winmain--the-application-entry-point
auto WINAPI wWinMain(HINSTANCE /* hinstance */,
                     HINSTANCE /* hprevinstance */,
                     PWSTR /* pcmdline */,
                     int /* ncmdshow */) -> int {
    auto window_manager { pane::window_manager() };
    auto window { make_window(window_manager) };
    auto window2 { make_window(window_manager) };
    auto webview { make_webview(window_manager) };

    return pane::system::message_loop();
}
