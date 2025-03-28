#include <pane/message_box.hxx>
#include <Windows.h>
#include <pane/hstring.hxx>

namespace pane {
auto message_box(string message, string title) -> int {
    auto converted_message { hstring::from_utf8(message.get()) };
    auto converted_title { hstring::from_utf8(title.get()) };

    if (!converted_message || !converted_title) {
        return 0;
    }

    return MessageBoxW(nullptr,
                       converted_message.value().c_str(),
                       converted_title.value().c_str(),
                       MB_OK | MB_ICONASTERISK);
}

auto error_box(string message, string title) -> int {
    auto converted_message { hstring::from_utf8(message.get()) };
    auto converted_title { hstring::from_utf8(title.get()) };

    if (!converted_message || !converted_title) {
        return 0;
    }

    return MessageBoxW(nullptr,
                       converted_message.value().c_str(),
                       converted_title.value().c_str(),
                       MB_OK | MB_ICONHAND);
}
} // namespace pane
