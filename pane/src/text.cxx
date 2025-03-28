#include <pane/text.hxx>
#include "icu.hxx"

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
} // namespace pane
