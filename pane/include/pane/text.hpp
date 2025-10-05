#pragma once
#include <format>
#include <expected>
#include <string_view>
#include <string>
#include <icu.h>

namespace pane {
auto as_u8string_view(const std::string& string) -> std::u8string_view;
auto as_string_view(std::u8string_view string) -> std::string_view;
auto as_u16string_view(const std::wstring& string) -> std::u16string_view;
auto as_wstring_view(std::u16string_view string) -> std::wstring_view;

auto as_c_str(const std::u8string& string) noexcept -> const char*;
auto as_c_str(std::u8string& string) noexcept -> char*;
auto as_c_str(const std::u16string& string) noexcept -> const wchar_t*;
auto as_c_str(std::u16string& string) noexcept -> wchar_t*;

auto to_utf8(std::u16string_view string) -> std::expected<std::u8string, UErrorCode>;
auto to_utf8(std::wstring_view string) -> std::expected<std::u8string, UErrorCode>;

auto to_utf8_lossy(std::u16string_view string, int32_t sub_char = 0xFFFD) -> std::u8string;
auto to_utf8_lossy(std::wstring_view string, int32_t sub_char = 0xFFFD) -> std::u8string;

auto to_utf16(std::u8string_view string) -> std::expected<std::u16string, UErrorCode>;
auto to_utf16(std::string_view string) -> std::expected<std::u16string, UErrorCode>;

auto to_utf16_lossy(std::u8string_view string, int32_t sub_char = 0xFFFD) -> std::u16string;
auto to_utf16_lossy(std::string_view string, int32_t sub_char = 0xFFFD) -> std::u16string;

auto validate_utf8(std::u8string_view string) -> bool;
auto validate_utf16(std::u16string_view string) -> bool;
} // namespace pane

namespace std {
template <> struct formatter<std::u8string_view> : formatter<string_view> {
    auto format(std::u8string_view string, format_context& context) const noexcept {
        return formatter<string_view>::format(
            std::string_view { reinterpret_cast<const char*>(string.data()), string.length() },
            context);
    }
};

template <size_t size> struct formatter<char8_t[size]> : formatter<u8string_view> {
    auto format(const char8_t* string, format_context& context) const noexcept {
        return formatter<u8string_view>::format(std::u8string_view { string, (size - 1) }, context);
    }
};

template <> struct formatter<const char8_t*> : formatter<u8string_view> {
    auto format(const char8_t* string, format_context& context) const noexcept {
        return formatter<u8string_view>::format(std::u8string_view { string }, context);
    }
};

template <> struct formatter<std::u8string> : formatter<u8string_view> {
    auto format(const std::u8string& string, format_context& context) const noexcept {
        return formatter<u8string_view>::format(
            std::u8string_view { string.data(), string.length() }, context);
    }
};

template <> struct formatter<std::u16string_view, wchar_t> : formatter<wstring_view, wchar_t> {
    auto format(std::u16string_view string, wformat_context& context) const {
        return formatter<wstring_view, wchar_t>::format(
            std::wstring_view { reinterpret_cast<const wchar_t*>(string.data()), string.length() },
            context);
    }
};

template <size_t size>
struct formatter<char16_t[size], wchar_t> : formatter<u16string_view, wchar_t> {
    auto format(const char16_t* string, wformat_context& context) const {
        return formatter<u16string_view, wchar_t>::format(
            std::u16string_view { string, (size - 1) }, context);
    }
};

template <> struct formatter<const char16_t*, wchar_t> : formatter<u16string_view, wchar_t> {
    auto format(const char16_t* string, wformat_context& context) const noexcept {
        return formatter<u16string_view, wchar_t>::format(std::u16string_view { string }, context);
    }
};

template <> struct formatter<std::u16string, wchar_t> : formatter<u16string_view, wchar_t> {
    auto format(const std::u16string& string, wformat_context& context) const {
        return formatter<u16string_view, wchar_t>::format(
            std::u16string_view { string.data(), string.length() }, context);
    }
};
} // namespace std
