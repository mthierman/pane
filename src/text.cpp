#include <pane/text.hpp>
#include "icu.h"

namespace pane {
[[nodiscard]]
constexpr auto string_view_cast(std::string_view s) -> std::u8string_view {
    return { reinterpret_cast<const char8_t*>(s.data()), s.size() };
}

[[nodiscard]]
constexpr auto string_view_cast(std::u8string_view s) -> std::string_view {
    return { reinterpret_cast<const char*>(s.data()), s.size() };
}

[[nodiscard]]
constexpr auto string_view_cast(std::wstring_view s) -> std::u16string_view {
    static_assert(sizeof(wchar_t) == 2, "wchar_t must be 16-bit for this cast");
    return { reinterpret_cast<const char16_t*>(s.data()), s.size() };
}

[[nodiscard]]
constexpr auto string_view_cast(std::u16string_view s) -> std::wstring_view {
    static_assert(sizeof(wchar_t) == 2, "wchar_t must be 16-bit for this cast");
    return { reinterpret_cast<const wchar_t*>(s.data()), s.size() };
}

auto to_utf8(std::u16string_view string) -> std::expected<std::u8string, UErrorCode> {
    int32_t length { 0 };
    auto error_code { U_ZERO_ERROR };

    u_strToUTF8(
        nullptr, 0, &length, string.data(), static_cast<int32_t>(string.length()), &error_code);

    if (!(error_code == U_BUFFER_OVERFLOW_ERROR || U_SUCCESS(error_code))) {
        return std::unexpected(error_code);
    }

    std::u8string buffer(length, u8'\0');
    error_code = U_ZERO_ERROR;

    u_strToUTF8(reinterpret_cast<char*>(buffer.data()),
                static_cast<int32_t>(buffer.length()),
                &length,
                string.data(),
                static_cast<int32_t>(string.length()),
                &error_code);

    if (U_FAILURE(error_code)) {
        return std::unexpected(error_code);
    }

    buffer.resize(length);

    return buffer;
}

auto to_utf8(std::wstring_view string) -> std::expected<std::u8string, UErrorCode> {
    return to_utf8(string_view_cast(string));
}

auto to_utf8_lossy(std::u16string_view string, int32_t sub_char) -> std::u8string {
    int32_t length { 0 };
    auto error_code { U_ZERO_ERROR };

    u_strToUTF8WithSub(nullptr,
                       0,
                       &length,
                       string.data(),
                       static_cast<int32_t>(string.length()),
                       0xFFFD,
                       nullptr,
                       &error_code);

    if (!(error_code == U_BUFFER_OVERFLOW_ERROR || U_SUCCESS(error_code))) {
        return {};
    }

    std::u8string buffer(length, 0);
    error_code = U_ZERO_ERROR;

    u_strToUTF8WithSub(reinterpret_cast<char*>(buffer.data()),
                       static_cast<int32_t>(buffer.length()),
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

    return buffer;
}

auto to_utf8_lossy(std::wstring_view string, int32_t /* sub_char */) -> std::u8string {
    return to_utf8_lossy(string_view_cast(string));
}

auto to_utf16(std::u8string_view string) -> std::expected<std::u16string, UErrorCode> {
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

    std::u16string buffer(length, 0);
    error_code = U_ZERO_ERROR;

    u_strFromUTF8(buffer.data(),
                  static_cast<int32_t>(buffer.length()),
                  &length,
                  reinterpret_cast<const char*>(string.data()),
                  static_cast<int32_t>(string.length()),
                  &error_code);

    if (U_FAILURE(error_code)) {
        return std::unexpected(error_code);
    }

    buffer.resize(length);

    return buffer;
}

auto to_utf16(std::string_view string) -> std::expected<std::u16string, UErrorCode> {
    return to_utf16(string_view_cast(string));
}

auto to_utf16_lossy(std::u8string_view string, int32_t sub_char) -> std::u16string {
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

    std::u16string buffer(length, 0);
    error_code = U_ZERO_ERROR;

    u_strFromUTF8WithSub(buffer.data(),
                         static_cast<int32_t>(buffer.length()),
                         &length,
                         reinterpret_cast<const char*>(string.data()),
                         static_cast<int32_t>(string.length()),
                         0xFFFD,
                         nullptr,
                         &error_code);

    if (U_FAILURE(error_code)) {
        return {};
    }

    buffer.resize(length);

    return buffer;
}

auto to_utf16_lossy(std::string_view string, int32_t /* sub_char */) -> std::u16string {
    return to_utf16_lossy(string_view_cast(string));
}

auto validate_utf8(std::u8string_view string) -> std::expected<void, UErrorCode> {
    int32_t length { 0 };
    auto error_code { U_ZERO_ERROR };

    u_strFromUTF8(nullptr,
                  0,
                  &length,
                  reinterpret_cast<const char*>(string.data()),
                  static_cast<int32_t>(string.length()),
                  &error_code);

    if (U_FAILURE(error_code)) {
        return std::unexpected(error_code);
    }

    return {};
}

auto validate_utf16(std::u16string_view string) -> std::expected<void, UErrorCode> {
    int32_t length { 0 };
    auto error_code { U_ZERO_ERROR };

    u_strToUTF8(
        nullptr, 0, &length, string.data(), static_cast<int32_t>(string.length()), &error_code);

    if (U_FAILURE(error_code)) {
        return std::unexpected(error_code);
    }

    return {};
}
} // namespace pane
