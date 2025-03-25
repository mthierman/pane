#include <pane/string.hxx>
#include <pane/hstring.hxx>
#include <new>

namespace pane {
string::string(std::u8string&& string) noexcept
    : storage { std::move(string) } { }

auto string::operator=(std::u8string&& string) noexcept -> Self& {
    storage = std::move(string);

    return *this;
}

string::string(const char8_t* string)
    : storage { string } { }

string::string(const std::u8string& string)
    : storage { string } { }

auto string::operator=(const std::u8string& string) -> Self& {
    storage = string;

    return *this;
}

string::string(const char* string)
    : storage { reinterpret_cast<const char8_t*>(string) } { }

string::string(const std::string& string)
    : storage { string.begin(), string.end() } { }

auto string::operator=(const std::string& string) -> Self& {
    storage = std::u8string { string.begin(), string.end() };

    return *this;
}

auto string::from_utf16(std::u16string_view string, bool replacement)
    -> std::expected<Self, std::error_code> {
    auto buffer { std::u8string() };
    auto errorCode { U_ZERO_ERROR };

    try {
        buffer.resize(string.length());
    } catch (const std::bad_alloc&) {
        return std::unexpected(std::make_error_code(std::errc::not_enough_memory));
    }

    u_strToUTF8WithSub(reinterpret_cast<char*>(buffer.data()),
                       static_cast<int32_t>(buffer.length()),
                       nullptr,
                       string.data(),
                       static_cast<int32_t>(string.length()),
                       replacement ? U_SENTINEL : 0xFFFD,
                       nullptr,
                       &errorCode);

    if (U_SUCCESS(errorCode)) {
        return Self(buffer);
    }

    return std::unexpected(make_error_code(errorCode));
}

auto string::from_utf16(std::wstring_view string, bool replacement)
    -> std::expected<Self, std::error_code> {
    auto buffer { std::u8string() };
    auto errorCode { U_ZERO_ERROR };

    try {
        buffer.resize(string.length());
    } catch (const std::bad_alloc&) {
        return std::unexpected(std::make_error_code(std::errc::not_enough_memory));
    }

    u_strToUTF8WithSub(reinterpret_cast<char*>(buffer.data()),
                       static_cast<int32_t>(buffer.length()),
                       nullptr,
                       reinterpret_cast<const char16_t*>(string.data()),
                       static_cast<int32_t>(string.length()),
                       replacement ? U_SENTINEL : 0xFFFD,
                       nullptr,
                       &errorCode);

    if (U_SUCCESS(errorCode)) {
        return Self(buffer);
    }

    return std::unexpected(make_error_code(errorCode));
}

auto string::from_utf16(const hstring& string, bool replacement)
    -> std::expected<Self, std::error_code> {
    return string::from_utf16(string.storage, replacement);
}

auto string::c_str() -> char* { return reinterpret_cast<char*>(storage.data()); }

auto string::c_str() const -> const char* { return reinterpret_cast<const char*>(storage.data()); }

auto string::u8_str() -> char8_t* { return storage.data(); }

auto string::u8_str() const -> const char8_t* { return storage.data(); }

auto string::to_string() -> std::string { return { storage.begin(), storage.end() }; }

auto string::to_string() const -> const std::string { return { storage.begin(), storage.end() }; }
} // namespace pane
