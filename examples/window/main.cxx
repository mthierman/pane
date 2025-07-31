#include <pane/pane.hxx>
#include <map>
#include <chrono>
#include <memory>

enum struct message : int {
    NEW_WINDOW = WM_APP,
};

struct window_manager final {
    using Self = window_manager;

    auto add(this Self& self) -> void {
        auto window { std::make_unique<pane::window>(
            pane::window_config { u8"window",
                                  pane::color { 0, 0, 0, 255 },
                                  pane::color { 255, 255, 255, 255 },
                                  true,
                                  nullptr },
            [&](const pane::window_message& window_message, pane::window& window) -> LRESULT {
            switch (window_message.msg) {
                case WM_CREATE: {
                } break;

                case WM_CLOSE: {
                    self.remove(window.window_handle());
                } break;

                case WM_KEYDOWN: {
                    switch (window_message.wparam) {
                        case 'N': {
                            if (pane::input::is_key_down(VK_LCONTROL)
                                || pane::input::is_key_down(VK_RCONTROL)) {
                                self.add();
                            }
                        } break;
                        case 'W': {
                            if (pane::input::is_key_down(VK_LCONTROL)
                                || pane::input::is_key_down(VK_RCONTROL)) {
                                self.remove(window.window_handle());
                            }
                        } break;
                    }
                } break;
            }

            return window.default_procedure(window_message);
        }) };

        self.windows.emplace(window->window_handle(), std::move(window));
    }

    auto remove(this Self& self, HWND hwnd) -> void {
        self.windows.erase(hwnd);

        if (self.windows.empty()) {
            PostQuitMessage(0);
        }
    }

    std::map<HWND, std::unique_ptr<pane::window>> windows;
};

// https://learn.microsoft.com/en-us/windows/win32/learnwin32/winmain--the-application-entry-point
auto wWinMain(HINSTANCE /* hinstance */,
              HINSTANCE /* hprevinstance */,
              PWSTR /* pcmdline */,
              int /* ncmdshow */) -> int {
    window_manager window_manager;

    pane::message_window app { [&](const pane::window_message& window_message,
                                   pane::message_window& app) -> LRESULT {
        if (window_message.msg == +message::NEW_WINDOW) {
            window_manager.add();
        }

        return app.default_procedure(window_message);
    } };

    pane::window_message { app.window_handle(), +message::NEW_WINDOW, 0, 0 }.send();

    return pane::system::message_loop();
}
