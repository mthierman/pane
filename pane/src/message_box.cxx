#include <pane/message_box.hxx>
#include <Windows.h>
#include <pane/text.hxx>

namespace pane::message_box {
auto info(std::u8string message, std::u8string title) -> int {
    return MessageBoxW(nullptr,
                       reinterpret_cast<const wchar_t*>(pane::to_utf16(message).data()),
                       reinterpret_cast<const wchar_t*>(pane::to_utf16(title).data()),
                       MB_OK | MB_ICONASTERISK);
}

auto error(std::u8string message, std::u8string title) -> int {
    return MessageBoxW(nullptr,
                       reinterpret_cast<const wchar_t*>(pane::to_utf16(message).data()),
                       reinterpret_cast<const wchar_t*>(pane::to_utf16(title).data()),
                       MB_OK | MB_ICONHAND);
}
} // namespace pane::message_box
