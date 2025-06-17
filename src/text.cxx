#include <pane/text.hxx>
#include "icu.h"

namespace pane {
auto to_utf16(std::u8string_view string) -> std::u16string {
    auto buffer { std::u16string() };
    auto error_code { U_ZERO_ERROR };

    buffer.resize(string.length());

    u_strFromUTF8WithSub(buffer.data(),
                         static_cast<int32_t>(buffer.length()),
                         nullptr,
                         reinterpret_cast<const char*>(string.data()),
                         static_cast<int32_t>(string.length()),
                         0xFFFD,
                         nullptr,
                         &error_code);

    return buffer;
}

auto to_utf16(std::string_view string) -> std::u16string {
    return to_utf16(reinterpret_cast<const char8_t*>(string.data()));
}

auto to_utf8(std::u16string_view string) -> std::u8string {
    auto buffer { std::u8string() };
    auto error_code { U_ZERO_ERROR };

    buffer.resize(string.length());

    u_strToUTF8WithSub(reinterpret_cast<char*>(buffer.data()),
                       static_cast<int32_t>(buffer.length()),
                       nullptr,
                       string.data(),
                       static_cast<int32_t>(string.length()),
                       0xFFFD,
                       nullptr,
                       &error_code);

    return buffer;
}

auto to_utf8(std::wstring_view string) -> std::u8string {
    return to_utf8(reinterpret_cast<const char16_t*>(string.data()));
}

auto to_u8string(std::string_view string) -> std::u8string {
    return { string.begin(), string.end() };
}

auto to_u16string(std::wstring_view string) -> std::u16string {
    return { string.begin(), string.end() };
}
} // namespace pane
