#pragma once
#include <format>
#include <string>
#include <pane/system.hxx>
#include <glaze/glaze.hpp>

namespace pane {
struct error final {
    using Self = error;

    error() = default;
    ~error() = default;

    error(const Self& error) = default;
    auto operator=(const Self& error) -> Self& = default;

    error(Self&& error) noexcept = default;
    auto operator=(Self&& error) noexcept -> Self& = default;

    explicit error(HRESULT error);
    explicit error(glz::error_ctx error);

    int64_t error_code;
    std::u8string message;
};
} // namespace pane
