#pragma once
#include <Windows.h>
#include <objidl.h>
#include <gdiplus.h>

namespace pane {
struct gdi_plus final {
    using Self = gdi_plus;

    gdi_plus();
    ~gdi_plus();

    gdi_plus(const Self&) = delete;
    auto operator=(const Self&) -> Self& = delete;

    gdi_plus(Self&&) noexcept = delete;
    auto operator=(Self&&) noexcept -> Self& = delete;

    Gdiplus::Status status;

private:
    Gdiplus::GdiplusStartupInput startup_input;
    ULONG_PTR token;
};
} // namespace pane
