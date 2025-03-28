#include <pane/message_box.hxx>
#include <Windows.h>
#include <pane/text.hxx>

namespace pane {
auto message_box(std::u8string message, std::u8string title) -> int {
    auto converted_message { pane::to_utf16(message) };
    auto converted_title { pane::to_utf16(title) };

    return MessageBoxW(nullptr,
                       reinterpret_cast<const wchar_t*>(converted_message.data()),
                       reinterpret_cast<const wchar_t*>(converted_title.data()),
                       MB_OK | MB_ICONASTERISK);
}

auto error_box(std::u8string message, std::u8string title) -> int {
    auto converted_message { pane::to_utf16(message) };
    auto converted_title { pane::to_utf16(title) };

    return MessageBoxW(nullptr,
                       reinterpret_cast<const wchar_t*>(converted_message.data()),
                       reinterpret_cast<const wchar_t*>(converted_title.data()),
                       MB_OK | MB_ICONHAND);
}
} // namespace pane
