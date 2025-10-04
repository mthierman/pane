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

// auto to_utf16(std::u8string_view string) -> std::u16string {
//     auto buffer { std::u16string() };
//     buffer.resize(string.length());

//     auto error_code { U_ZERO_ERROR };
//     u_strFromUTF8WithSub(buffer.data(),
//                          static_cast<int32_t>(buffer.length()),
//                          nullptr,
//                          reinterpret_cast<const char*>(string.data()),
//                          static_cast<int32_t>(string.length()),
//                          0xFFFD,
//                          nullptr,
//                          &error_code);

//     return buffer;
// }

// auto to_utf16(std::string_view string) -> std::u16string {
//     return to_utf16(reinterpret_cast<const char8_t*>(string.data()));
// }

// auto to_utf8(std::u16string_view string) -> std::u8string {
//     auto buffer { std::u8string() };
//     buffer.resize(string.length());

//     auto error_code { U_ZERO_ERROR };
//     u_strToUTF8WithSub(reinterpret_cast<char*>(buffer.data()),
//                        static_cast<int32_t>(buffer.length()),
//                        nullptr,
//                        string.data(),
//                        static_cast<int32_t>(string.length()),
//                        0xFFFD,
//                        nullptr,
//                        &error_code);

//     return buffer;
// }

// auto to_utf8(std::wstring_view string) -> std::u8string {
//     return to_utf8(reinterpret_cast<const char16_t*>(string.data()));
// }

// auto to_u8string(std::string_view string) -> std::u8string {
//     return { string.begin(), string.end() };
// }

// auto to_u16string(std::wstring_view string) -> std::u16string {
//     return { string.begin(), string.end() };
// }

// auto to_string(std::u8string_view string) -> std::string {
//     return { string.begin(), string.end() };
// }

// auto to_wstring(std::u16string_view string) -> std::wstring {
//     return { string.begin(), string.end() };
// }
} // namespace pane
