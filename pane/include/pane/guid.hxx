#pragma once
#include <Windows.h>
#include <objbase.h>
#include <rpc.h>
#include <expected>
#include <optional>
#include <system_error>
#include <pane/string.hxx>

namespace pane {
struct guid {
    using Self = guid;

    ~guid() = default;

    guid(Self&& file) noexcept = default;
    auto operator=(Self&& file) noexcept -> Self& = default;

    guid(const Self& file) = default;
    auto operator=(const Self& file) -> Self& = default;

    static auto create() -> std::expected<Self, std::error_code>;

    auto to_string(this Self& self) -> std::optional<string>;

    GUID storage;

private:
    guid() = default;
};
} // namespace pane
