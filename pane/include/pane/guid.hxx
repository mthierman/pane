#pragma once
#include <Windows.h>
#include <objbase.h>
#include <rpc.h>
#include <expected>
#include <optional>
#include <string>
#include <system_error>

namespace pane {
struct guid {
    using Self = guid;

    guid() = delete;
    ~guid() = default;

    guid(const Self& guid) = default;
    auto operator=(const Self& guid) -> Self& = default;

    guid(Self&& guid) noexcept = default;
    auto operator=(Self&& guid) noexcept -> Self& = default;

    guid(GUID&& guid) noexcept;

    static auto create() -> std::expected<Self, std::error_code>;

    auto string(this Self& self) -> std::optional<std::u8string>;

    GUID storage;
};
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
} // namespace std
