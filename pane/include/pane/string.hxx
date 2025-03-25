#pragma once
#include <expected>
#include <format>
#include <string>
#include <system_error>
#include <pane/icu.hxx>

namespace pane {
struct hstring;
// https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p1423r3.html
struct string {
    using Self = string;

    string() = default;
    ~string() = default;

    string(Self&& string) noexcept = default;
    auto operator=(Self&& string) noexcept -> Self& = default;

    string(const Self& string) = default;
    auto operator=(const Self& string) -> Self& = default;

    explicit string(std::u8string&& string) noexcept;
    auto operator=(std::u8string&& string) noexcept -> Self&;

    explicit string(const char8_t* string);
    explicit string(const std::u8string& string);
    auto operator=(const std::u8string& string) -> Self&;

    explicit string(const char* string);
    explicit string(const std::string& string);
    auto operator=(const std::string& string) -> Self&;

    static auto from_utf16(std::u16string_view string, bool replacement = true)
        -> std::expected<Self, std::error_code>;

    static auto from_utf16(std::wstring_view string, bool replacement = true)
        -> std::expected<Self, std::error_code>;

    static auto from_utf16(const hstring& string, bool replacement = true)
        -> std::expected<Self, std::error_code>;

    auto c_str() -> char*;

    auto c_str() const -> const char*;

    auto u8_str() -> char8_t*;

    auto u8_str() const -> const char8_t*;

    auto to_string() -> std::string;

    auto to_string() const -> const std::string;

    std::u8string storage;
};
} // namespace pane

namespace std {
template <> struct formatter<std::u8string> : formatter<string_view> {
    auto format(const std::u8string& string, format_context& context) const noexcept {
        return formatter<string_view>::format(std::string { string.begin(), string.end() },
                                              context);
    }
};

template <> struct formatter<pane::string> : formatter<string_view> {
    auto format(const pane::string& string, format_context& context) const noexcept {
        return formatter<string_view>::format(string.to_string(), context);
    }
};
} // namespace std
