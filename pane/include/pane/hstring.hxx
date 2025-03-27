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
    auto operator=(const std::u16string& string) -> Self&;

    explicit hstring(std::wstring_view string);
    auto operator=(const std::wstring& string) -> Self&;

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

    auto to_wstring(this Self& self) -> std::wstring;

    auto to_wstring(this const Self& self) -> const std::wstring;

    std::u16string storage;
};
} // namespace pane

namespace std {
template <> struct formatter<std::u16string, wchar_t> : formatter<wstring_view, wchar_t> {
    auto format(const std::u16string& u16string, wformat_context& context) const noexcept {
        return formatter<wstring_view, wchar_t>::format(
            std::wstring { u16string.begin(), u16string.end() }, context);
    }
};

template <> struct formatter<pane::hstring, wchar_t> : formatter<wstring_view, wchar_t> {
    auto format(const pane::hstring& hstring, wformat_context& context) const noexcept {
        return formatter<wstring_view, wchar_t>::format(hstring.to_wstring(), context);
    }
};
} // namespace std
