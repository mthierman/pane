#include <pane/hstring.hxx>
#include <pane/string.hxx>
#include <new>

namespace pane {
hstring::hstring(std::u16string&& string) noexcept
    : storage { std::move(string) } { }

auto hstring::operator=(std::u16string&& string) noexcept -> Self& {
    storage = std::move(string);

    return *this;
}

hstring::hstring(std::u16string_view string)
    : storage { string } { }

auto hstring::operator=(const std::u16string& string) -> Self& {
    storage = string;

    return *this;
}

hstring::hstring(std::wstring_view string)
    : storage { string.begin(), string.end() } { }

auto hstring::operator=(const std::wstring& string) -> Self& {
    storage = { string.begin(), string.end() };

    return *this;
}

auto hstring::from_utf8(std::u8string_view string, bool replacement)
    -> std::expected<Self, std::error_code> {
    auto buffer { std::u16string() };
    auto errorCode { U_ZERO_ERROR };

    try {
        buffer.resize(string.length());
    } catch (const std::bad_alloc&) {
        return std::unexpected(std::make_error_code(std::errc::not_enough_memory));
    }

    u_strFromUTF8WithSub(buffer.data(),
                         static_cast<int32_t>(buffer.length()),
                         nullptr,
                         reinterpret_cast<const char*>(string.data()),
                         static_cast<int32_t>(string.length()),
                         replacement ? U_SENTINEL : 0xFFFD,
                         nullptr,
                         &errorCode);

    if (U_SUCCESS(errorCode)) {
        return Self(std::move(buffer));
    }

    return std::unexpected(make_error_code(errorCode));
}

auto hstring::from_utf8(std::string_view string, bool replacement)
    -> std::expected<Self, std::error_code> {
    return from_utf8(
        std::u8string_view { reinterpret_cast<const char8_t*>(string.data()), string.length() },
        replacement);
}

auto hstring::from_utf8(const string& string, bool replacement)
    -> std::expected<Self, std::error_code> {
    return from_utf8(string.storage, replacement);
}

auto hstring::c_str(this Self& self) -> wchar_t* {
    return reinterpret_cast<wchar_t*>(self.storage.data());
}

auto hstring::c_str(this const Self& self) -> const wchar_t* {
    return reinterpret_cast<const wchar_t*>(self.storage.data());
}

auto hstring::u16_str(this Self& self) -> char16_t* { return self.storage.data(); }

auto hstring::u16_str(this const Self& self) -> const char16_t* { return self.storage.data(); }

auto hstring::to_wstring(this Self& self) -> std::wstring {
    return { self.storage.begin(), self.storage.end() };
}

auto hstring::to_wstring(this const Self& self) -> const std::wstring {
    return { self.storage.begin(), self.storage.end() };
}
} // namespace pane
