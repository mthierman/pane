#pragma once
#include <Windows.h>
#include <rpc.h>
#include <format>
#include <optional>
#include <wil/win32_helpers.h>
#include <pane/text.hxx>

namespace pane {
auto new_guid() -> std::optional<GUID>;
} // namespace pane

namespace std {
template <> struct hash<GUID> {
    auto operator()(const GUID& guid) const noexcept {
        RPC_STATUS status { RPC_S_OK };
        return static_cast<size_t>(UuidHash(&const_cast<GUID&>(guid), &status));
    }
};

template <> struct less<GUID> {
    auto operator()(const GUID& lhs, const GUID& rhs) const noexcept {
        RPC_STATUS status { RPC_S_OK };
        return UuidCompare(&const_cast<GUID&>(lhs), &const_cast<GUID&>(rhs), &status) == -1 ? true
                                                                                            : false;
    }
};

template <> struct formatter<GUID> : formatter<string_view> {
    auto format(const GUID& guid, format_context& context) const noexcept {
        std::wstring buffer;
        buffer.resize(wil::guid_string_buffer_length);
        StringFromGUID2(guid, buffer.data(), wil::guid_string_buffer_length);

        auto converted_buffer { pane::to_utf8(buffer) };

        return formatter<string_view>::format(
            { reinterpret_cast<const char*>(converted_buffer.data()), converted_buffer.length() },
            context);
    }
};

template <> struct formatter<GUID, wchar_t> : formatter<wstring_view, wchar_t> {
    auto format(const GUID& guid, wformat_context& context) const noexcept {
        std::wstring buffer;
        buffer.resize(wil::guid_string_buffer_length);
        StringFromGUID2(guid, buffer.data(), wil::guid_string_buffer_length);

        return formatter<wstring_view, wchar_t>::format(buffer, context);
    }
};
} // namespace std
