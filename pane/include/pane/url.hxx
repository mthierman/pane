#pragma once
#include <optional>
#include <string>
#include <ada.h>
#include <pane/string.hxx>

namespace pane {
struct url {
    using Self = url;

    ~url() = default;

    url(Self&& url) noexcept = default;
    auto operator=(Self&& url) noexcept -> Self& = default;

    url(const Self& url) = default;
    auto operator=(const Self& url) -> Self& = default;

    static auto create(std::string_view string) -> std::optional<Self>;
    static auto create(std::u8string_view string) -> std::optional<Self>;
    static auto create(const string& string) -> std::optional<Self>;

    ada::url storage;

private:
    url() = default;
};
} // namespace pane
