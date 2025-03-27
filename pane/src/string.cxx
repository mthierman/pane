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

string::string(std::u8string_view string)
    : storage { string } { }

auto string::operator=(const std::u8string& string) -> Self& {
    storage = string;

    return *this;
}

string::string(std::string_view string)
    : storage { std::u8string_view(reinterpret_cast<const char8_t*>(string.data()),
                                   string.length()) } { }

auto string::operator=(const std::string& string) -> Self& {
    storage
        = std::u8string_view { reinterpret_cast<const char8_t*>(string.data()), string.length() };

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
    return from_utf16(reinterpret_cast<const char16_t*>(string.data()), replacement);
}

auto string::from_utf16(const hstring& string, bool replacement)
    -> std::expected<Self, std::error_code> {
    return from_utf16(string.storage, replacement);
}

auto string::c_str(this Self& self) -> char* {
    return reinterpret_cast<char*>(self.storage.data());
}

auto string::c_str(this const Self& self) -> const char* {
    return reinterpret_cast<const char*>(self.storage.data());
}

auto string::u8_str(this Self& self) -> char8_t* { return self.storage.data(); }

auto string::u8_str(this const Self& self) -> const char8_t* { return self.storage.data(); }

auto string::to_string(this Self& self) -> std::string {
    return { self.storage.begin(), self.storage.end() };
}

auto string::to_string(this const Self& self) -> const std::string {
    return { self.storage.begin(), self.storage.end() };
}
} // namespace pane
