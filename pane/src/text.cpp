#include <pane/text.hpp>
#include "icu.h"

namespace pane {
auto from_utf16(std::u16string_view string) -> std::expected<std::u8string, UErrorCode> {
    int32_t required_length { 0 };
    auto error_code { U_ZERO_ERROR };

    u_strToUTF8(nullptr, 0, &required_length, string.data(), string.length(), &error_code);

    if (U_FAILURE(error_code)) {
        return std::unexpected(error_code);
    }

    int32_t actual_length { 0 };
    auto buffer { std::u8string() };
    buffer.resize(required_length + 1);
    error_code = U_ZERO_ERROR;

    u_strToUTF8(reinterpret_cast<char*>(buffer.data()),
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

auto from_utf16(std::wstring_view string) -> std::expected<std::u8string, UErrorCode> {
    return from_utf16({ reinterpret_cast<const char16_t*>(string.data()), string.length() });
}

auto from_utf8(std::u8string_view string) -> std::expected<std::u16string, UErrorCode> {
    int32_t required_length { 0 };
    auto error_code { U_ZERO_ERROR };

    u_strFromUTF8(nullptr,
                  0,
                  &required_length,
                  reinterpret_cast<const char*>(string.data()),
                  string.length(),
                  &error_code);

    if (U_FAILURE(error_code)) {
        return std::unexpected(error_code);
    }

    int32_t actual_length { 0 };
    auto buffer { std::u16string() };
    buffer.resize(required_length + 1);
    error_code = U_ZERO_ERROR;

    u_strFromUTF8(buffer.data(),
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

auto from_utf8(std::string_view string) -> std::expected<std::u16string, UErrorCode> {
    return from_utf8({ reinterpret_cast<const char8_t*>(string.data()), string.length() });
}

auto c_str(const std::u8string& string) noexcept -> const char* {
    return reinterpret_cast<const char*>(string.data());
}

auto c_str(const std::u16string& string) noexcept -> const wchar_t* {
    return reinterpret_cast<const wchar_t*>(string.data());
}
} // namespace pane
