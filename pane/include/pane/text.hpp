#pragma once
#include <format>
#include <expected>
#include <string_view>
#include <string>
#include <icu.h>

namespace pane {
auto from_utf16(std::u16string_view string) -> std::expected<std::u8string, UErrorCode>;
auto from_utf16(std::wstring_view string) -> std::expected<std::u8string, UErrorCode>;

auto from_utf16_lossy(std::u16string_view string) -> std::u8string;
auto from_utf16_lossy(std::wstring_view string) -> std::u8string;

auto from_utf8(std::u8string_view string) -> std::expected<std::u16string, UErrorCode>;
auto from_utf8(std::string_view string) -> std::expected<std::u16string, UErrorCode>;

auto from_utf8_lossy(std::u8string_view string) -> std::u16string;
auto from_utf8_lossy(std::string_view string) -> std::u16string;

auto c_str(const std::u8string& string) noexcept -> const char*;
auto c_str(std::u8string& string) noexcept -> char*;
auto c_str(const std::u16string& string) noexcept -> const wchar_t*;
auto c_str(std::u16string& string) noexcept -> wchar_t*;
} // namespace pane

namespace std {
template <> struct formatter<const char8_t*> : formatter<string_view> {
    auto format(const char8_t* string, format_context& context) const noexcept {
        return formatter<string_view>::format(reinterpret_cast<const char*>(string), context);
    }
};

template <size_t size> struct formatter<char8_t[size]> : formatter<string_view> {
    auto format(const char8_t* string, format_context& context) const noexcept {
        return formatter<string_view>::format(reinterpret_cast<const char*>(string), context);
    }
};

template <> struct formatter<std::u8string> : formatter<string_view> {
    auto format(const std::u8string& string, format_context& context) const noexcept {
        return formatter<string_view>::format(
            { reinterpret_cast<const char*>(string.data()), string.length() }, context);
    }
};

template <> struct formatter<std::u8string_view> : formatter<string_view> {
    auto format(std::u8string_view string, format_context& context) const noexcept {
        return formatter<string_view>::format(
            { reinterpret_cast<const char*>(string.data()), string.length() }, context);
    }
};

template <> struct formatter<const char16_t*> : formatter<wstring_view, wchar_t> {
    auto format(const char16_t* string, wformat_context& context) const noexcept {
        return formatter<wstring_view, wchar_t>::format(reinterpret_cast<const wchar_t*>(string),
                                                        context);
    }
};

template <size_t size> struct formatter<char16_t[size]> : formatter<wstring_view, wchar_t> {
    auto format(const char16_t* string, wformat_context& context) const noexcept {
        return formatter<wstring_view, wchar_t>::format(reinterpret_cast<const wchar_t*>(string),
                                                        context);
    }
};

template <> struct formatter<std::u16string, wchar_t> : formatter<wstring_view, wchar_t> {
    auto format(const std::u16string& string, wformat_context& context) const noexcept {
        return formatter<wstring_view, wchar_t>::format(
            { reinterpret_cast<const wchar_t*>(string.data()), string.length() }, context);
    }
};

template <> struct formatter<std::u16string_view, wchar_t> : formatter<wstring_view, wchar_t> {
    auto format(std::u16string_view string, wformat_context& context) const noexcept {
        return formatter<wstring_view, wchar_t>::format(
            { reinterpret_cast<const wchar_t*>(string.data()), string.length() }, context);
    }
};
} // namespace std
