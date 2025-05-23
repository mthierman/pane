#pragma once
#include <expected>
#include <format>
#include <string>
#include <string_view>
#include <system_error>
#include "../../src/icu.hxx"

namespace pane {
struct hstring;
// https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p1423r3.html
struct string final {
    using Self = string;

    string() = default;
    ~string() = default;

    string(const Self&) = default;
    auto operator=(const Self&) -> Self& = default;

    string(Self&&) noexcept = default;
    auto operator=(Self&&) noexcept -> Self& = default;

    string(std::u8string&& string) noexcept;
    auto operator=(std::u8string&& string) noexcept -> Self&;

    string(const char8_t* string);
    auto operator=(const char8_t* string) -> Self&;
    string(const std::u8string& string);
    auto operator=(const std::u8string& string) -> Self&;
    string(std::u8string_view string);
    auto operator=(std::u8string_view string) -> Self&;

    string(const char* string);
    auto operator=(const char* string) -> Self&;
    string(const std::string& string);
    auto operator=(const std::string& string) -> Self&;
    string(std::string_view string);
    auto operator=(std::string_view string) -> Self&;

    static auto from_utf16(std::u16string_view string, bool replacement = true)
        -> std::expected<Self, std::error_code>;

    static auto from_utf16(std::wstring_view string, bool replacement = true)
        -> std::expected<Self, std::error_code>;

    auto c_str(this Self& self) -> char*;

    auto c_str(this const Self& self) -> const char*;

    auto data(this Self& self) -> char8_t*;

    auto data(this const Self& self) -> const char8_t*;

    auto length(this const Self& self) -> size_t;

    auto get(this const Self& self) -> const std::u8string&;

private:
    std::u8string storage;
};
} // namespace pane

namespace std {
template <> struct formatter<std::u8string> : formatter<string_view> {
    auto format(const std::u8string& string, format_context& context) const noexcept {
        return formatter<string_view>::format(
            { reinterpret_cast<const char*>(string.data()), string.length() }, context);
    }
};

template <> struct formatter<pane::string> : formatter<string_view> {
    auto format(const pane::string& string, format_context& context) const noexcept {
        return formatter<string_view>::format(
            { reinterpret_cast<const char*>(string.data()), string.length() }, context);
    }
};
} // namespace std
