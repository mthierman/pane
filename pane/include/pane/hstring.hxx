#pragma once
#include <expected>
#include <format>
#include <string>
#include <system_error>
#include "../../src/icu.hxx"

namespace pane {
struct string;
// https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p1423r3.html
struct hstring {
    using Self = hstring;

    hstring() = default;
    ~hstring() = default;

    hstring(Self&& string) noexcept = default;
    auto operator=(Self&& string) noexcept -> Self& = default;

    hstring(const Self& string) = default;
    auto operator=(const Self& string) -> Self& = default;

    explicit hstring(std::u16string&& string) noexcept;
    auto operator=(std::u16string&& string) noexcept -> Self&;

    explicit hstring(std::u16string_view string);
    auto operator=(const std::u16string_view& string) -> Self&;

    explicit hstring(std::wstring_view string);
    auto operator=(const std::wstring_view& string) -> Self&;

    static auto from_utf8(std::u8string_view string, bool replacement = true)
        -> std::expected<Self, std::error_code>;

    static auto from_utf8(std::string_view string, bool replacement = true)
        -> std::expected<Self, std::error_code>;

    static auto from_utf8(const string& string, bool replacement = true)
        -> std::expected<Self, std::error_code>;

    auto c_str(this Self& self) -> wchar_t*;

    auto c_str(this const Self& self) -> const wchar_t*;

    auto u16_str(this Self& self) -> char16_t*;

    auto u16_str(this const Self& self) -> const char16_t*;

    auto to_std_wstring(this Self& self) -> std::wstring;

    auto to_std_wstring(this const Self& self) -> const std::wstring;

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
