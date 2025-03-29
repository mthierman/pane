#pragma once
#include <Windows.h>
#include <objidl.h>
#include <gdiplus.h>

namespace pane {
struct gdi_plus {
    using Self = gdi_plus;

    gdi_plus();
    ~gdi_plus();

    gdi_plus(Self&& gdi_plus) noexcept = delete;
    auto operator=(Self&& gdi_plus) noexcept -> Self& = delete;

    gdi_plus(const Self& gdi_plus) = delete;
    auto operator=(const Self& gdi_plus) -> Self& = delete;

    Gdiplus::Status status;

private:
    Gdiplus::GdiplusStartupInput startup_input;
    ULONG_PTR token;
};
} // namespace pane
