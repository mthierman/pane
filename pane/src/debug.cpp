#include <pane/debug.hpp>

namespace pane {
auto debug(const std::string& string) -> void {
    const auto u16string { pane::to_utf16_lossy(string) };

    OutputDebugStringW(reinterpret_cast<const wchar_t*>(u16string.data()));
    OutputDebugStringW(L"\n");
}

auto debug(const std::u8string& string) -> void {
    const auto u16string { pane::to_utf16_lossy(string) };

    OutputDebugStringW(reinterpret_cast<const wchar_t*>(u16string.data()));
    OutputDebugStringW(L"\n");
}

auto debug(const std::wstring& string) -> void {
    OutputDebugStringW(string.data());
    OutputDebugStringW(L"\n");
}

auto debug(const std::u16string& string) -> void {
    OutputDebugStringW(reinterpret_cast<const wchar_t*>(string.data()));
    OutputDebugStringW(L"\n");
}
} // namespace pane
