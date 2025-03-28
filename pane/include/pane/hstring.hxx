#pragma once
#include <expected>
#include <format>
#include <string>
#include <string_view>
#include <system_error>
#include "../../src/icu.hxx"

namespace pane {
struct string;
// https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p1423r3.html
struct hstring {
    using Self = hstring;

    hstring() = default;
    ~hstring() = default;

    hstring(const Self& string) = default;
    auto operator=(const Self& string) -> Self& = default;

    hstring(Self&& string) noexcept = default;
    auto operator=(Self&& string) noexcept -> Self& = default;
    hstring(std::u16string&& string) noexcept;
    auto operator=(std::u16string&& string) noexcept -> Self&;

    hstring(const char16_t* string);
    auto operator=(const char16_t* string) -> Self&;
    hstring(const std::u16string& string);
    auto operator=(const std::u16string& string) -> Self&;
    hstring(std::u16string_view string);
    auto operator=(std::u16string_view string) -> Self&;

    hstring(const wchar_t* string);
    auto operator=(const wchar_t* string) -> Self&;
    hstring(const std::wstring& string);
    auto operator=(const std::wstring& string) -> Self&;
    hstring(std::wstring_view string);
    auto operator=(std::wstring_view string) -> Self&;

    static auto from_utf8(std::u8string_view string, bool replacement = true)
        -> std::expected<Self, std::error_code>;

    static auto from_utf8(std::string_view string, bool replacement = true)
        -> std::expected<Self, std::error_code>;

    auto c_str(this Self& self) -> wchar_t*;

    auto c_str(this const Self& self) -> const wchar_t*;

    auto u16_str(this Self& self) -> char16_t*;

    auto u16_str(this const Self& self) -> const char16_t*;

    std::u16string storage;
};
} // namespace pane

namespace std {
template <> struct formatter<std::u16string, wchar_t> : formatter<wstring_view, wchar_t> {
    auto format(const std::u16string& string, wformat_context& context) const noexcept {
        return formatter<wstring_view, wchar_t>::format(
            { reinterpret_cast<const wchar_t*>(string.data()), string.size() }, context);
    }
};

template <> struct formatter<pane::hstring, wchar_t> : formatter<wstring_view, wchar_t> {
    auto format(const pane::hstring& string, wformat_context& context) const noexcept {
        return formatter<wstring_view, wchar_t>::format(
            { reinterpret_cast<const wchar_t*>(string.storage.data()), string.storage.length() },
            context);
    }
};
} // namespace std
