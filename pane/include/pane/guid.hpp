#pragma once
#include <Windows.h>
#include <rpc.h>
#include <format>
#include <optional>
#include <wil/win32_helpers.h>
#include <pane/text.hpp>

namespace pane {
auto guid() -> std::optional<GUID>;
} // namespace pane

namespace std {
template <> struct hash<GUID> {
    auto operator()(const GUID& guid) const {
        RPC_STATUS status { RPC_S_OK };
        return static_cast<size_t>(UuidHash(&const_cast<GUID&>(guid), &status));
    }
};

template <> struct less<GUID> {
    auto operator()(const GUID& lhs, const GUID& rhs) const {
        RPC_STATUS status { RPC_S_OK };
        return UuidCompare(&const_cast<GUID&>(lhs), &const_cast<GUID&>(rhs), &status) == -1 ? true
                                                                                            : false;
    }
};

template <> struct formatter<GUID> : formatter<string_view> {
    auto format(const GUID& guid, format_context& context) const {
        wchar_t wbuffer[wil::guid_string_buffer_length];
        StringFromGUID2(guid, wbuffer, wil::guid_string_buffer_length);

        char buffer[wil::guid_string_buffer_length];
        auto length { WideCharToMultiByte(
            CP_UTF8, 0, wbuffer, -1, buffer, sizeof(buffer), nullptr, nullptr) };

        return formatter<string_view>::format(
            std::string_view { buffer, static_cast<std::string_view::size_type>(length - 1) },
            context);
    }
};

template <> struct formatter<GUID, wchar_t> : formatter<wstring_view, wchar_t> {
    auto format(const GUID& guid, wformat_context& context) const {
        wchar_t wbuffer[wil::guid_string_buffer_length];
        StringFromGUID2(guid, wbuffer, wil::guid_string_buffer_length);

        return formatter<wstring_view, wchar_t>::format(wbuffer, context);
    }
};
} // namespace std
