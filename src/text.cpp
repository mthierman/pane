#include <pane/text.hpp>
#include "icu.h"
#include <vector>

namespace pane {
auto to_utf8(std::u16string_view string) -> std::expected<std::u8string, UErrorCode> {
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

auto to_utf8(std::wstring_view string) -> std::expected<std::u8string, UErrorCode> {
    return to_utf8(reinterpret_string_view(string));
}

auto to_utf8_lossy(std::u16string_view string, int32_t sub_char) -> std::u8string {
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

auto to_utf8_lossy(std::wstring_view string, int32_t sub_char) -> std::u8string {
    return to_utf8_lossy(reinterpret_string_view(string), sub_char);
}

auto to_utf16(std::u8string_view string) -> std::expected<std::u16string, UErrorCode> {
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

auto to_utf16(std::string_view string) -> std::expected<std::u16string, UErrorCode> {
    return to_utf16(reinterpret_string_view(string));
}

auto to_utf16_lossy(std::u8string_view string, int32_t sub_char) -> std::u16string {
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

auto to_utf16_lossy(std::string_view string, int32_t sub_char) -> std::u16string {
    return to_utf16_lossy(reinterpret_string_view(string), sub_char);
}

auto validate_utf8(std::u8string_view string) -> std::expected<void, UErrorCode> {
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

auto validate_utf8(std::string_view string) -> std::expected<void, UErrorCode> {
    return validate_utf8(reinterpret_string_view(string));
}

auto validate_utf16(std::u16string_view string) -> std::expected<void, UErrorCode> {
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

auto validate_utf16(std::wstring_view string) -> std::expected<void, UErrorCode> {
    return validate_utf16(reinterpret_string_view(string));
}
} // namespace pane
