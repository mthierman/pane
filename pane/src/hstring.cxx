#include <pane/hstring.hxx>
#include <pane/string.hxx>
#include <new>

namespace pane {
hstring::hstring(std::u16string&& str) noexcept
    : storage { std::move(str) } { }

auto hstring::operator=(std::u16string&& str) noexcept -> Self& {
    storage = std::move(str);

    return *this;
}

hstring::hstring(const char16_t* str)
    : storage { str } { }

hstring::hstring(const std::u16string& str)
    : storage { str } { }

auto hstring::operator=(const std::u16string& str) -> Self& {
    storage = str;

    return *this;
}

hstring::hstring(const wchar_t* str)
    : storage { reinterpret_cast<const char16_t*>(str) } { }

hstring::hstring(const std::wstring& str)
    : storage { str.begin(), str.end() } { }

auto hstring::operator=(const std::wstring& str) -> Self& {
    storage = std::u16string { str.begin(), str.end() };

    return *this;
}

auto hstring::from_utf8(std::u8string_view str, bool replacement)
    -> std::expected<Self, std::error_code> {
    auto buffer { std::u16string() };
    auto errorCode { U_ZERO_ERROR };

    try {
        buffer.resize(str.length());
    } catch (const std::bad_alloc&) {
        return std::unexpected(std::make_error_code(std::errc::not_enough_memory));
    }

    u_strFromUTF8WithSub(buffer.data(),
                         static_cast<int32_t>(buffer.length()),
                         nullptr,
                         reinterpret_cast<const char*>(str.data()),
                         static_cast<int32_t>(str.length()),
                         replacement ? U_SENTINEL : 0xFFFD,
                         nullptr,
                         &errorCode);

    if (U_SUCCESS(errorCode)) {
        return Self(buffer);
    }

    return std::unexpected(make_error_code(errorCode));
}

auto hstring::from_utf8(std::string_view str, bool replacement)
    -> std::expected<Self, std::error_code> {
    auto buffer { std::u16string() };
    auto errorCode { U_ZERO_ERROR };

    try {
        buffer.resize(str.length());
    } catch (const std::bad_alloc&) {
        return std::unexpected(std::make_error_code(std::errc::not_enough_memory));
    }

    u_strFromUTF8WithSub(buffer.data(),
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

auto hstring::from_utf8(const string& str, bool replacement)
    -> std::expected<Self, std::error_code> {
    return hstring::from_utf8(str.storage, replacement);
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
