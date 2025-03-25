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

hstring::hstring(const char16_t* string)
    : storage { string } { }

hstring::hstring(const std::u16string& string)
    : storage { string } { }

auto hstring::operator=(const std::u16string& string) -> Self& {
    storage = string;

    return *this;
}

hstring::hstring(const wchar_t* string)
    : storage { reinterpret_cast<const char16_t*>(string) } { }

hstring::hstring(const std::wstring& string)
    : storage { string.begin(), string.end() } { }

auto hstring::operator=(const std::wstring& string) -> Self& {
    storage = std::u16string { string.begin(), string.end() };

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
        return Self(buffer);
    }

    return std::unexpected(make_error_code(errorCode));
}

auto hstring::from_utf8(std::string_view string, bool replacement)
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

auto hstring::from_utf8(const string& string, bool replacement)
    -> std::expected<Self, std::error_code> {
    return hstring::from_utf8(string.storage, replacement);
}

auto hstring::c_str() -> wchar_t* { return reinterpret_cast<wchar_t*>(storage.data()); }

auto hstring::c_str() const -> const wchar_t* {
    return reinterpret_cast<const wchar_t*>(storage.data());
}

auto hstring::u16_str() -> char16_t* { return storage.data(); }

auto hstring::u16_str() const -> const char16_t* { return storage.data(); }

auto hstring::to_wstring() -> std::wstring { return { storage.begin(), storage.end() }; }

auto hstring::to_wstring() const -> const std::wstring {
    return { storage.begin(), storage.end() };
}
} // namespace pane
