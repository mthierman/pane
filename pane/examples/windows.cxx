#include <pane/pane.hxx>

// https://learn.microsoft.com/en-us/windows/win32/learnwin32/winmain--the-application-entry-point
auto WINAPI wWinMain(HINSTANCE /* hinstance */,
                     HINSTANCE /* hprevinstance */,
                     PWSTR /* pcmdline */,
                     int /* ncmdshow */) -> int {
    auto webview { pane::webview({}, { .home_page = u8"https://www.google.com/" }) };

    // auto window { pane::window() };
    // window.~window();

    // auto win { std::make_unique<pane::window>() };

    return pane::system::message_loop();
}
