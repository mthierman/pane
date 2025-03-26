#pragma once
#include <ada.h>

namespace pane {
struct url {
    using Self = url;

    url() = default;
    ~url() = default;

    url(Self&& str) noexcept = default;
    auto operator=(Self&& str) noexcept -> Self& = default;

    url(const Self& str) = default;
    auto operator=(const Self& str) -> Self& = default;
};
} // namespace pane
