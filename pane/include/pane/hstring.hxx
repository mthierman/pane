#pragma once
#include <expected>
#include <format>
#include <string>
#include <system_error>
#include <pane/icu.hxx>

namespace pane {
struct string;
// https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p1423r3.html
struct hstring {
    using Self = hstring;

    hstring() = default;
    ~hstring() = default;

    hstring(Self&& str) noexcept = default;
    auto operator=(Self&& str) noexcept -> Self& = default;

    hstring(const Self& str) = default;
    auto operator=(const Self& str) -> Self& = default;

    explicit hstring(std::u16string&& str) noexcept;
    auto operator=(std::u16string&& str) noexcept -> Self&;

    explicit hstring(const char16_t* str);
    explicit hstring(const std::u16string& str);
    auto operator=(const std::u16string& str) -> Self&;

    explicit hstring(const wchar_t* str);
    explicit hstring(const std::wstring& str);
    auto operator=(const std::wstring& str) -> Self&;

    static auto from_utf8(std::u8string_view str, bool replacement = true)
        -> std::expected<Self, std::error_code>;

    static auto from_utf8(std::string_view str, bool replacement = true)
        -> std::expected<Self, std::error_code>;

    static auto from_utf8(const string& str, bool replacement = true)
        -> std::expected<Self, std::error_code>;

    auto c_str() -> wchar_t*;

    auto c_str() const -> const wchar_t*;

    auto u16_str() -> char16_t*;

    auto u16_str() const -> const char16_t*;

    auto to_wstring() -> std::wstring;

    auto to_wstring() const -> const std::wstring;

    std::u16string storage;
};
} // namespace pane

namespace std {
template <> struct formatter<std::u16string, wchar_t> : formatter<wstring_view, wchar_t> {
    auto format(const std::u16string& str, wformat_context& context) const noexcept {
        return formatter<wstring_view, wchar_t>::format(std::wstring { str.begin(), str.end() },
                                                        context);
    }
};

template <> struct formatter<pane::hstring, wchar_t> : formatter<wstring_view, wchar_t> {
    auto format(const pane::hstring& str, wformat_context& context) const noexcept {
        return formatter<wstring_view, wchar_t>::format(str.to_wstring(), context);
    }
};
} // namespace std
