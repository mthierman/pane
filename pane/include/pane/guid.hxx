#pragma once
#include <Windows.h>
#include <rpc.h>

namespace pane {
struct guid {
    using Self = guid;

    guid() = default;
    ~guid() = default;

    guid(Self&& file) noexcept = default;
    auto operator=(Self&& file) noexcept -> Self& = default;

    guid(const Self& file) = default;
    auto operator=(const Self& file) -> Self& = default;
};
} // namespace pane
