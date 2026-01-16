#pragma once
#include <format>
#include <expected>
#include <string>
#include <string_view>
#include <icu.h>

// https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p1423r3.html
namespace pane {
[[nodiscard]]
constexpr auto string_view_cast(std::string_view string) noexcept -> std::u8string_view {
    return { reinterpret_cast<const char8_t*>(string.data()), string.length() };
}

[[nodiscard]]
constexpr auto string_view_cast(std::u8string_view string) noexcept -> std::string_view {
    return { reinterpret_cast<const char*>(string.data()), string.length() };
}

[[nodiscard]]
constexpr auto string_view_cast(std::wstring_view string) noexcept -> std::u16string_view {
    static_assert(sizeof(wchar_t) == 2, "wchar_t must be 16-bit for this cast");
    return { reinterpret_cast<const char16_t*>(string.data()), string.length() };
}

[[nodiscard]]
constexpr auto string_view_cast(std::u16string_view string) noexcept -> std::wstring_view {
    static_assert(sizeof(wchar_t) == 2, "wchar_t must be 16-bit for this cast");
    return { reinterpret_cast<const wchar_t*>(string.data()), string.length() };
}

auto to_utf8(std::u16string_view string) -> std::expected<std::u8string, UErrorCode>;
auto to_utf8(std::wstring_view string) -> std::expected<std::u8string, UErrorCode>;

auto to_utf8_lossy(std::u16string_view string, int32_t sub_char = 0xFFFD) -> std::u8string;
auto to_utf8_lossy(std::wstring_view string, int32_t sub_char = 0xFFFD) -> std::u8string;

auto to_utf16(std::u8string_view string) -> std::expected<std::u16string, UErrorCode>;
auto to_utf16(std::string_view string) -> std::expected<std::u16string, UErrorCode>;

auto to_utf16_lossy(std::u8string_view string, int32_t sub_char = 0xFFFD) -> std::u16string;
auto to_utf16_lossy(std::string_view string, int32_t sub_char = 0xFFFD) -> std::u16string;

auto validate_utf8(std::u8string_view string) -> std::expected<void, UErrorCode>;
auto validate_utf8(std::string_view string) -> std::expected<void, UErrorCode>;

auto validate_utf16(std::u16string_view string) -> std::expected<void, UErrorCode>;
auto validate_utf16(std::wstring_view string) -> std::expected<void, UErrorCode>;
} // namespace pane

namespace std {
template <> struct formatter<std::u8string_view> : formatter<string_view> {
    auto format(std::u8string_view string, format_context& context) const noexcept {
        return formatter<string_view>::format(pane::string_view_cast(string), context);
    }
};

template <size_t size> struct formatter<char8_t[size]> : formatter<u8string_view> {
    auto format(const char8_t* string, format_context& context) const noexcept {
        return formatter<u8string_view>::format(string, context);
    }
};

template <> struct formatter<const char8_t*> : formatter<u8string_view> {
    auto format(const char8_t* string, format_context& context) const noexcept {
        return formatter<u8string_view>::format(string, context);
    }
};

template <> struct formatter<std::u8string> : formatter<u8string_view> {
    auto format(const std::u8string& string, format_context& context) const noexcept {
        return formatter<u8string_view>::format(string, context);
    }
};

template <> struct formatter<std::u16string_view, wchar_t> : formatter<wstring_view, wchar_t> {
    auto format(std::u16string_view string, wformat_context& context) const {
        return formatter<wstring_view, wchar_t>::format(pane::string_view_cast(string), context);
    }
};

template <size_t size>
struct formatter<char16_t[size], wchar_t> : formatter<u16string_view, wchar_t> {
    auto format(const char16_t* string, wformat_context& context) const {
        return formatter<u16string_view, wchar_t>::format(string, context);
    }
};

template <> struct formatter<const char16_t*, wchar_t> : formatter<u16string_view, wchar_t> {
    auto format(const char16_t* string, wformat_context& context) const noexcept {
        return formatter<u16string_view, wchar_t>::format(string, context);
    }
};

template <> struct formatter<std::u16string, wchar_t> : formatter<u16string_view, wchar_t> {
    auto format(const std::u16string& string, wformat_context& context) const {
        return formatter<u16string_view, wchar_t>::format(string, context);
    }
};
} // namespace std
