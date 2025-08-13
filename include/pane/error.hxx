#pragma once
#include <format>
#include <string>
#include <pane/system.hxx>
#include <pane/glaze.hxx>
#include <icu.h>

namespace pane {
struct error final {
    using Self = error;

    error() = default;
    ~error() = default;

    error(const Self&) = default;
    auto operator=(const Self&) -> Self& = default;

    error(Self&&) noexcept = default;
    auto operator=(Self&&) noexcept -> Self& = default;

    explicit error(HRESULT error_code);
    explicit error(glz::error_code error_code);
    explicit error(UErrorCode error_code);

    int64_t error_code;
    std::u8string message;
};
} // namespace pane
