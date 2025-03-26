#pragma once
#include <optional>
#include <ada.h>
#include <pane/string.hxx>

namespace pane {
struct url {
    using Self = url;

    url() = default;
    ~url() = default;

    url(Self&& str) noexcept = default;
    auto operator=(Self&& str) noexcept -> Self& = default;

    url(const Self& str) = default;
    auto operator=(const Self& str) -> Self& = default;

    static auto from_string(string string) -> std::optional<Self>;

    ada::url storage;
};
} // namespace pane
