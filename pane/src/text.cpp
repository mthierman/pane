#include <pane/text.hpp>
#include "icu.h"

namespace pane {
auto as_u8string_view(std::string_view string) -> std::u8string_view {
    return std::u8string_view { reinterpret_cast<const char8_t*>(string.data()), string.length() };
}

auto as_string_view(std::u8string_view string) -> std::string_view {
    return std::string_view { reinterpret_cast<const char*>(string.data()), string.length() };
}

auto as_u16string_view(std::wstring_view string) -> std::u16string_view {
    return std::u16string_view { reinterpret_cast<const char16_t*>(string.data()),
                                 string.length() };
}

auto as_wstring_view(std::u16string_view string) -> std::wstring_view {
    return std::wstring_view { reinterpret_cast<const wchar_t*>(string.data()), string.length() };
}

auto as_c_str(const std::u8string& string) noexcept -> const char* {
    return reinterpret_cast<const char*>(string.data());
}

auto as_c_str(std::u8string& string) noexcept -> char* {
    return reinterpret_cast<char*>(string.data());
}

auto as_c_str(const std::u16string& string) noexcept -> const wchar_t* {
    return reinterpret_cast<const wchar_t*>(string.data());
}

auto as_c_str(std::u16string& string) noexcept -> wchar_t* {
    return reinterpret_cast<wchar_t*>(string.data());
}

auto to_utf8(std::u16string_view string) -> std::expected<std::u8string, UErrorCode> {
    int32_t required_length { 0 };
    auto error_code { U_ZERO_ERROR };

    [[maybe_unused]] auto result { u_strToUTF8(
        nullptr, 0, &required_length, string.data(), string.length(), &error_code) };

    if (U_FAILURE(error_code)) {
        return std::unexpected(error_code);
    }

    int32_t actual_length { 0 };
    auto buffer { std::u8string() };
    buffer.resize(required_length + 1);
    error_code = U_ZERO_ERROR;

    result = u_strToUTF8(as_c_str(buffer),
                         buffer.length(),
                         &actual_length,
                         string.data(),
                         string.length(),
                         &error_code);

    if (U_FAILURE(error_code)) {
        return std::unexpected(error_code);
    }

    return buffer;
}

auto to_utf8(std::wstring_view string) -> std::expected<std::u8string, UErrorCode> {
    return to_utf8(as_u16string_view(string));
}

auto to_utf8_lossy(std::u16string_view string, int32_t sub_char) -> std::u8string {
    int32_t required_length { 0 };
    auto error_code { U_ZERO_ERROR };

    [[maybe_unused]] auto result { u_strToUTF8WithSub(nullptr,
                                                      0,
                                                      &required_length,
                                                      string.data(),
                                                      string.length(),
                                                      0xFFFD,
                                                      nullptr,
                                                      &error_code) };

    int32_t actual_length { 0 };
    auto buffer { std::u8string() };
    buffer.resize(required_length + 1);
    error_code = U_ZERO_ERROR;

    result = u_strToUTF8WithSub(reinterpret_cast<char*>(buffer.data()),
                                buffer.length(),
                                &actual_length,
                                string.data(),
                                string.length(),
                                sub_char,
                                nullptr,
                                &error_code);

    return buffer;
}

auto to_utf8_lossy(std::wstring_view string, int32_t sub_char) -> std::u8string {
    return to_utf8_lossy(as_u16string_view(string));
}

auto to_utf16(std::u8string_view string) -> std::expected<std::u16string, UErrorCode> {
    int32_t required_length { 0 };
    auto error_code { U_ZERO_ERROR };

    [[maybe_unused]] auto result { u_strFromUTF8(nullptr,
                                                 0,
                                                 &required_length,
                                                 reinterpret_cast<const char*>(string.data()),
                                                 string.length(),
                                                 &error_code) };

    if (U_FAILURE(error_code)) {
        return std::unexpected(error_code);
    }

    int32_t actual_length { 0 };
    auto buffer { std::u16string() };
    buffer.resize(required_length + 1);
    error_code = U_ZERO_ERROR;

    result = u_strFromUTF8(buffer.data(),
                           buffer.length(),
                           &actual_length,
                           reinterpret_cast<const char*>(string.data()),
                           string.length(),
                           &error_code);

    if (U_FAILURE(error_code)) {
        return std::unexpected(error_code);
    }

    return buffer;
}

auto to_utf16(std::string_view string) -> std::expected<std::u16string, UErrorCode> {
    return to_utf16(as_u8string_view(string));
}

auto to_utf16_lossy(std::u8string_view string, int32_t sub_char) -> std::u16string {
    int32_t required_length { 0 };
    auto error_code { U_ZERO_ERROR };

    [[maybe_unused]] auto result { u_strFromUTF8WithSub(
        nullptr,
        0,
        &required_length,
        reinterpret_cast<const char*>(string.data()),
        string.length(),
        sub_char,
        nullptr,
        &error_code) };

    int32_t actual_length { 0 };
    auto buffer { std::u16string() };
    buffer.resize(required_length + 1);
    error_code = U_ZERO_ERROR;

    result = u_strFromUTF8WithSub(buffer.data(),
                                  buffer.length(),
                                  &actual_length,
                                  reinterpret_cast<const char*>(string.data()),
                                  string.length(),
                                  0xFFFD,
                                  nullptr,
                                  &error_code);

    return buffer;
}

auto to_utf16_lossy(std::string_view string, int32_t sub_char) -> std::u16string {
    return to_utf16_lossy(as_u8string_view(string));
}

auto validate_utf8(std::u8string_view string) -> bool {
    int32_t required_length { 0 };
    auto error_code { U_ZERO_ERROR };

    [[maybe_unused]] auto result { u_strFromUTF8(nullptr,
                                                 0,
                                                 &required_length,
                                                 reinterpret_cast<const char*>(string.data()),
                                                 string.length(),
                                                 &error_code) };

    return U_SUCCESS(error_code);
}

auto validate_utf16(std::u16string_view string) -> bool {
    int32_t required_length { 0 };
    auto error_code { U_ZERO_ERROR };

    [[maybe_unused]] auto result { u_strToUTF8(
        nullptr, 0, &required_length, string.data(), string.length(), &error_code) };

    return U_SUCCESS(error_code);
}
} // namespace pane
