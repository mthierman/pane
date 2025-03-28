#pragma once
#include <optional>
#include <string>
#include <ada.h>
#include <pane/string.hxx>

namespace pane {
struct url {
    using Self = url;

    url() = delete;
    ~url() = default;

    url(const Self& url) = default;
    auto operator=(const Self& url) -> Self& = default;

    url(Self&& url) noexcept = default;
    auto operator=(Self&& url) noexcept -> Self& = default;

    url(ada::url&& url) noexcept;

    static auto create(const string& string) -> std::optional<Self>;

    ada::url storage;

private:
};
} // namespace pane
