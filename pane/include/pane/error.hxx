#pragma once
#include <format>
#include <string>
#include <pane/system.hxx>

namespace pane {
template <typename T> struct error final {
    using Self = error;

    error() = default;
    ~error() = default;

    error(const Self& error) = default;
    auto operator=(const Self& error) -> Self& = default;

    error(Self&& error) noexcept = default;
    auto operator=(Self&& error) noexcept -> Self& = default;

    explicit error(HRESULT hresult)
        : error_code { hresult },
          message { pane::system::format_message(hresult) } { }

    T error_code;
    std::u8string message;
};
} // namespace pane
