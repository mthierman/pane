#include <pane/text.hpp>
#include "icu.h"

namespace pane {
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
    int32_t length { 0 };
    auto error_code { U_ZERO_ERROR };

    [[maybe_unused]] auto result { u_strToUTF8(
        nullptr, 0, &length, string.data(), string.length(), &error_code) };

    if (U_FAILURE(error_code)) {
        return std::unexpected(error_code);
    }

    std::u8string buffer(length, u8'\0');
    error_code = U_ZERO_ERROR;

    result = u_strToUTF8(
        as_c_str(buffer), buffer.length(), &length, string.data(), string.length(), &error_code);

    if (U_FAILURE(error_code)) {
        return std::unexpected(error_code);
    }

    return buffer;
}

auto to_utf8(std::wstring_view string) -> std::expected<std::u8string, UErrorCode> {
    return to_utf8({ reinterpret_cast<const char16_t*>(string.data()), string.length() });
}

auto to_utf8_lossy(std::u16string_view string, int32_t sub_char) -> std::u8string {
    int32_t length { 0 };
    auto error_code { U_ZERO_ERROR };

    [[maybe_unused]] auto result { u_strToUTF8WithSub(
        nullptr, 0, &length, string.data(), string.length(), 0xFFFD, nullptr, &error_code) };

    std::u8string buffer(length, 0);
    error_code = U_ZERO_ERROR;

    result = u_strToUTF8WithSub(reinterpret_cast<char*>(buffer.data()),
                                buffer.length(),
                                &length,
                                string.data(),
                                string.length(),
                                sub_char,
                                nullptr,
                                &error_code);

    return buffer;
}

auto to_utf8_lossy(std::wstring_view string, int32_t sub_char) -> std::u8string {
    return to_utf8_lossy({ reinterpret_cast<const char16_t*>(string.data()), string.length() });
}

auto to_utf16(std::u8string_view string) -> std::expected<std::u16string, UErrorCode> {
    int32_t length { 0 };
    auto error_code { U_ZERO_ERROR };

    [[maybe_unused]] auto result { u_strFromUTF8(nullptr,
                                                 0,
                                                 &length,
                                                 reinterpret_cast<const char*>(string.data()),
                                                 string.length(),
                                                 &error_code) };

    if (U_FAILURE(error_code)) {
        return std::unexpected(error_code);
    }

    std::u16string buffer(length, 0);
    error_code = U_ZERO_ERROR;

    result = u_strFromUTF8(buffer.data(),
                           buffer.length(),
                           &length,
                           reinterpret_cast<const char*>(string.data()),
                           string.length(),
                           &error_code);

    if (U_FAILURE(error_code)) {
        return std::unexpected(error_code);
    }

    return buffer;
}

auto to_utf16(std::string_view string) -> std::expected<std::u16string, UErrorCode> {
    return to_utf16({ reinterpret_cast<const char8_t*>(string.data()), string.length() });
}

auto to_utf16_lossy(std::u8string_view string, int32_t sub_char) -> std::u16string {
    int32_t length { 0 };
    auto error_code { U_ZERO_ERROR };

    [[maybe_unused]] auto result { u_strFromUTF8WithSub(
        nullptr,
        0,
        &length,
        reinterpret_cast<const char*>(string.data()),
        string.length(),
        sub_char,
        nullptr,
        &error_code) };

    std::u16string buffer(length, 0);
    error_code = U_ZERO_ERROR;

    result = u_strFromUTF8WithSub(buffer.data(),
                                  buffer.length(),
                                  &length,
                                  reinterpret_cast<const char*>(string.data()),
                                  string.length(),
                                  0xFFFD,
                                  nullptr,
                                  &error_code);

    return buffer;
}

auto to_utf16_lossy(std::string_view string, int32_t sub_char) -> std::u16string {
    return to_utf16_lossy({ reinterpret_cast<const char8_t*>(string.data()), string.length() });
}

auto validate_utf8(std::u8string_view string) -> bool {
    int32_t length { 0 };
    auto error_code { U_ZERO_ERROR };

    [[maybe_unused]] auto result { u_strFromUTF8(nullptr,
                                                 0,
                                                 &length,
                                                 reinterpret_cast<const char*>(string.data()),
                                                 string.length(),
                                                 &error_code) };

    return U_SUCCESS(error_code);
}

auto validate_utf16(std::u16string_view string) -> bool {
    int32_t length { 0 };
    auto error_code { U_ZERO_ERROR };

    [[maybe_unused]] auto result { u_strToUTF8(
        nullptr, 0, &length, string.data(), string.length(), &error_code) };

    return U_SUCCESS(error_code);
}
} // namespace pane
