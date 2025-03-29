#pragma once
#include <format>
#include <string_view>
#include <string>

namespace pane {
auto to_utf16(std::u8string_view string) -> std::u16string;
auto to_utf16(std::string_view string) -> std::u16string;
auto to_utf8(std::u16string_view string) -> std::u8string;
auto to_utf8(std::wstring_view string) -> std::u8string;
} // namespace pane

namespace std {
template <> struct formatter<std::u8string> : formatter<string_view> {
    auto format(const std::u8string& string, format_context& context) const noexcept {
        return formatter<string_view>::format(
            { reinterpret_cast<const char*>(string.data()), string.length() }, context);
    }
};

template <> struct formatter<std::u16string, wchar_t> : formatter<wstring_view, wchar_t> {
    auto format(const std::u16string& string, wformat_context& context) const noexcept {
        return formatter<wstring_view, wchar_t>::format(
            { reinterpret_cast<const wchar_t*>(string.data()), string.length() }, context);
    }
};
} // namespace std
