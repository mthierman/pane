#pragma once
#include <icu.h>
#include <expected>
#include <format>
#include <string>

namespace pane {
struct hstring;
// https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p1423r3.html
struct string {
    using Self = string;

    string() = default;
    ~string() = default;

    string(Self&& str) noexcept = default;
    auto operator=(Self&& str) noexcept -> Self& = default;

    string(const Self& str) = default;
    auto operator=(const Self& str) -> Self& = default;

    explicit string(std::u8string&& str) noexcept;
    auto operator=(std::u8string&& str) noexcept -> Self&;

    explicit string(const char8_t* str);
    explicit string(const std::u8string& str);
    auto operator=(const std::u8string& str) -> Self&;

    explicit string(const char* str);
    explicit string(const std::string& str);
    auto operator=(const std::string& str) -> Self&;

    static auto from_utf16(std::u16string_view str, bool replacement = true)
        -> std::expected<Self, std::u8string>;

    static auto from_utf16(std::wstring_view str, bool replacement = true)
        -> std::expected<Self, std::u8string>;

    static auto from_utf16(const hstring& str, bool replacement = true)
        -> std::expected<Self, std::u8string>;

    auto c_str() -> char*;

    auto c_str() const -> const char*;

    auto u8_str() -> char8_t*;

    auto u8_str() const -> const char8_t*;

    std::u8string data;
};
} // namespace pane

namespace std {
template <> struct formatter<std::u8string> : formatter<string_view> {
    auto format(const std::u8string& str, format_context& context) const noexcept {
        return formatter<string_view>::format(std::string { str.begin(), str.end() }, context);
    }
};

template <> struct formatter<pane::string> : formatter<string_view> {
    auto format(const pane::string& str, format_context& context) const noexcept {
        return formatter<string_view>::format(std::string { str.data.begin(), str.data.end() },
                                              context);
    }
};
} // namespace std
