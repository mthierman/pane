#pragma once
#include <Windows.h>
#include <objbase.h>
#include <rpc.h>
#include <expected>
#include <system_error>

namespace pane {
struct guid {
    using Self = guid;

    ~guid() = default;

    guid(Self&& file) noexcept = default;
    auto operator=(Self&& file) noexcept -> Self& = default;

    guid(const Self& file) = default;
    auto operator=(const Self& file) -> Self& = default;

    static auto create() -> std::expected<Self, std::error_code>;

    GUID storage;

private:
    guid() = default;
};
} // namespace pane
