#include <pane/debug.hxx>

namespace pane {
auto debug(const std::u8string& string) -> void {
    OutputDebugStringW(reinterpret_cast<const wchar_t*>(pane::to_utf16(string).data()));
    OutputDebugStringW(L"\n");
}

auto debug(const std::u16string& string) -> void {
    OutputDebugStringW(reinterpret_cast<const wchar_t*>(string.data()));
    OutputDebugStringW(L"\n");
}
} // namespace pane
