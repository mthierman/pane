#include <pane/string.hxx>
#include <pane/hstring.hxx>
#include <new>

namespace pane {
string::string(std::u8string&& str) noexcept
    : data { std::move(str) } { }

auto string::operator=(std::u8string&& str) noexcept -> Self& {
    data = std::move(str);

    return *this;
}

string::string(const char8_t* str)
    : data { str } { }

string::string(const std::u8string& str)
    : data { str } { }

auto string::operator=(const std::u8string& str) -> Self& {
    data = str;

    return *this;
}

string::string(const char* str)
    : data { reinterpret_cast<const char8_t*>(str) } { }

string::string(const std::string& str)
    : data { str.begin(), str.end() } { }

auto string::operator=(const std::string& str) -> Self& {
    data = std::u8string { str.begin(), str.end() };

    return *this;
}

auto string::from_utf16(std::u16string_view str, bool replacement)
    -> std::expected<Self, std::error_code> {
    auto buffer { std::u8string() };
    auto errorCode { U_ZERO_ERROR };

    try {
        buffer.resize(str.length());
    } catch (const std::bad_alloc&) {
        return std::unexpected(std::make_error_code(std::errc::not_enough_memory));
    }

    u_strToUTF8WithSub(reinterpret_cast<char*>(buffer.data()),
                       static_cast<int32_t>(buffer.length()),
                       nullptr,
                       str.data(),
                       static_cast<int32_t>(str.length()),
                       replacement ? U_SENTINEL : 0xFFFD,
                       nullptr,
                       &errorCode);

    if (U_SUCCESS(errorCode)) {
        return Self(buffer);
    }

    return std::unexpected(make_error_code(errorCode));
}

auto string::from_utf16(std::wstring_view str, bool replacement)
    -> std::expected<Self, std::error_code> {
    auto buffer { std::u8string() };
    auto errorCode { U_ZERO_ERROR };

    try {
        buffer.resize(str.length());
    } catch (const std::bad_alloc&) {
        return std::unexpected(std::make_error_code(std::errc::not_enough_memory));
    }

    u_strToUTF8WithSub(reinterpret_cast<char*>(buffer.data()),
                       static_cast<int32_t>(buffer.length()),
                       nullptr,
                       reinterpret_cast<const char16_t*>(str.data()),
                       static_cast<int32_t>(str.length()),
                       replacement ? U_SENTINEL : 0xFFFD,
                       nullptr,
                       &errorCode);

    if (U_SUCCESS(errorCode)) {
        return Self(buffer);
    }

    return std::unexpected(make_error_code(errorCode));
}

auto string::from_utf16(const hstring& str, bool replacement)
    -> std::expected<Self, std::error_code> {
    return string::from_utf16(str.data, replacement);
}

auto string::c_str() -> char* { return reinterpret_cast<char*>(data.data()); }

auto string::c_str() const -> const char* { return reinterpret_cast<const char*>(data.data()); }

auto string::u8_str() -> char8_t* { return data.data(); }

auto string::u8_str() const -> const char8_t* { return data.data(); }
} // namespace pane
