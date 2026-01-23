#include <pane/text.hpp>
#include "icu.h"
#include <vector>

namespace pane {
auto utf16_to_utf8(std::u16string_view string) -> std::expected<std::u8string, UErrorCode> {
    if (string.empty()) {
        return {};
    }

    int32_t length { 0 };
    auto error_code { U_ZERO_ERROR };

    u_strToUTF8(
        nullptr, 0, &length, string.data(), static_cast<int32_t>(string.length()), &error_code);

    if (!(error_code == U_BUFFER_OVERFLOW_ERROR || U_SUCCESS(error_code))) {
        return std::unexpected(error_code);
    }

    std::vector<char> buffer(length);
    error_code = U_ZERO_ERROR;

    u_strToUTF8(buffer.data(),
                static_cast<int32_t>(buffer.size()),
                &length,
                string.data(),
                static_cast<int32_t>(string.length()),
                &error_code);

    if (U_FAILURE(error_code)) {
        return std::unexpected(error_code);
    }

    buffer.resize(length);

    return std::u8string { reinterpret_cast<const char8_t*>(buffer.data()), buffer.size() };
}

auto utf16_to_utf8(std::wstring_view string) -> std::expected<std::u8string, UErrorCode> {
    return utf16_to_utf8(reinterpret_string_view(string));
}

auto utf16_to_utf8_replace(std::u16string_view string, int32_t sub_char) -> std::u8string {
    if (string.empty()) {
        return {};
    }

    int32_t length { 0 };
    auto error_code { U_ZERO_ERROR };

    u_strToUTF8WithSub(nullptr,
                       0,
                       &length,
                       string.data(),
                       static_cast<int32_t>(string.length()),
                       sub_char,
                       nullptr,
                       &error_code);

    if (!(error_code == U_BUFFER_OVERFLOW_ERROR || U_SUCCESS(error_code))) {
        return {};
    }

    std::vector<char> buffer(length);
    error_code = U_ZERO_ERROR;

    u_strToUTF8WithSub(buffer.data(),
                       static_cast<int32_t>(buffer.size()),
                       &length,
                       string.data(),
                       static_cast<int32_t>(string.length()),
                       sub_char,
                       nullptr,
                       &error_code);

    if (U_FAILURE(error_code)) {
        return {};
    }

    buffer.resize(length);

    return std::u8string { reinterpret_cast<const char8_t*>(buffer.data()), buffer.size() };
}

auto utf16_to_utf8_replace(std::wstring_view string, int32_t sub_char) -> std::u8string {
    return utf16_to_utf8_replace(reinterpret_string_view(string), sub_char);
}

auto utf8_to_utf16(std::u8string_view string) -> std::expected<std::u16string, UErrorCode> {
    if (string.empty()) {
        return {};
    }

    int32_t length { 0 };
    auto error_code { U_ZERO_ERROR };

    u_strFromUTF8(nullptr,
                  0,
                  &length,
                  reinterpret_cast<const char*>(string.data()),
                  static_cast<int32_t>(string.length()),
                  &error_code);

    if (!(error_code == U_BUFFER_OVERFLOW_ERROR || U_SUCCESS(error_code))) {
        return std::unexpected(error_code);
    }

    std::vector<char16_t> buffer(length);
    error_code = U_ZERO_ERROR;

    u_strFromUTF8(buffer.data(),
                  static_cast<int32_t>(buffer.size()),
                  &length,
                  reinterpret_string_view(string).data(),
                  static_cast<int32_t>(string.length()),
                  &error_code);

    if (U_FAILURE(error_code)) {
        return std::unexpected(error_code);
    }

    buffer.resize(length);

    return std::u16string { buffer.data(), buffer.size() };
}

auto utf8_to_utf16(std::string_view string) -> std::expected<std::u16string, UErrorCode> {
    return utf8_to_utf16(reinterpret_string_view(string));
}

auto utf8_to_utf16_replace(std::u8string_view string, int32_t sub_char) -> std::u16string {
    if (string.empty()) {
        return {};
    }

    int32_t length { 0 };
    auto error_code { U_ZERO_ERROR };

    u_strFromUTF8WithSub(nullptr,
                         0,
                         &length,
                         reinterpret_cast<const char*>(string.data()),
                         static_cast<int32_t>(string.length()),
                         sub_char,
                         nullptr,
                         &error_code);

    if (!(error_code == U_BUFFER_OVERFLOW_ERROR || U_SUCCESS(error_code))) {
        return {};
    }

    std::vector<char16_t> buffer(length);
    error_code = U_ZERO_ERROR;

    u_strFromUTF8WithSub(buffer.data(),
                         static_cast<int32_t>(buffer.size()),
                         &length,
                         reinterpret_string_view(string).data(),
                         static_cast<int32_t>(string.length()),
                         sub_char,
                         nullptr,
                         &error_code);

    if (U_FAILURE(error_code)) {
        return {};
    }

    buffer.resize(length);

    return std::u16string { buffer.data(), buffer.size() };
}

auto utf8_to_utf16_replace(std::string_view string, int32_t sub_char) -> std::u16string {
    return utf8_to_utf16_replace(reinterpret_string_view(string), sub_char);
}

auto check_utf8(std::u8string_view string) -> std::expected<void, UErrorCode> {
    if (string.empty()) {
        return {};
    }

    int32_t length { 0 };
    auto error_code { U_ZERO_ERROR };

    u_strFromUTF8(nullptr,
                  0,
                  &length,
                  reinterpret_string_view(string).data(),
                  static_cast<int32_t>(string.length()),
                  &error_code);

    if (U_FAILURE(error_code)) {
        return std::unexpected(error_code);
    }

    return {};
}

auto check_utf8(std::string_view string) -> std::expected<void, UErrorCode> {
    return check_utf8(reinterpret_string_view(string));
}

auto check_utf16(std::u16string_view string) -> std::expected<void, UErrorCode> {
    if (string.empty()) {
        return {};
    }

    int32_t length { 0 };
    auto error_code { U_ZERO_ERROR };

    u_strToUTF8(
        nullptr, 0, &length, string.data(), static_cast<int32_t>(string.length()), &error_code);

    if (U_FAILURE(error_code)) {
        return std::unexpected(error_code);
    }

    return {};
}

auto check_utf16(std::wstring_view string) -> std::expected<void, UErrorCode> {
    return check_utf16(reinterpret_string_view(string));
}
} // namespace pane
