#pragma once
#include <Windows.h>
#include <rpc.h>
#include <expected>

namespace pane {
struct guid {
    using Self = guid;

    ~guid() = default;

    guid(Self&& file) noexcept = default;
    auto operator=(Self&& file) noexcept -> Self& = default;

    guid(const Self& file) = default;
    auto operator=(const Self& file) -> Self& = default;

    auto create() -> std::expected<Self, std::error_code>;

    GUID storage;

private:
    guid() = default;
};
} // namespace pane
