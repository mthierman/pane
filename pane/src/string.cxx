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

auto string::operator=(const char8_t* string) -> Self& {
    storage = string;

    return *this;
}

string::string(const std::u8string& string)
    : storage { string } { }

auto string::operator=(const std::u8string& string) -> Self& {
    storage = string;

    return *this;
}

string::string(std::u8string_view string)
    : storage { string } { }

auto string::operator=(std::u8string_view string) -> Self& {
    storage = string;

    return *this;
}

string::string(const char* string)
    : storage { reinterpret_cast<const char8_t*>(string) } { }

auto string::operator=(const char* string) -> Self& {
    storage = reinterpret_cast<const char8_t*>(string);

    return *this;
}

string::string(const std::string& string)
    : storage { string.begin(), string.end() } { }

auto string::operator=(const std::string& string) -> Self& {
    storage = { string.begin(), string.end() };

    return *this;
}

string::string(std::string_view string)
    : storage { string.begin(), string.end() } { }

auto string::operator=(std::string_view string) -> Self& {
    storage = { string.begin(), string.end() };

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
        return Self(std::move(buffer));
    }

    return std::unexpected(make_error_code(errorCode));
}

auto string::from_utf16(std::wstring_view string, bool replacement)
    -> std::expected<Self, std::error_code> {
    return from_utf16(
        std::u16string_view { reinterpret_cast<const char16_t*>(string.data()), string.length() },
        replacement);
}

auto string::c_str(this Self& self) -> char* {
    return reinterpret_cast<char*>(self.storage.data());
}

auto string::c_str(this const Self& self) -> const char* {
    return reinterpret_cast<const char*>(self.storage.data());
}

auto string::data(this Self& self) -> char8_t* { return self.storage.data(); }

auto string::data(this const Self& self) -> const char8_t* { return self.storage.data(); }

auto string::length(this const Self& self) -> const size_t { return self.storage.length(); }

auto string::get(this const Self& self) -> const std::u8string& { return self.storage; }
} // namespace pane
