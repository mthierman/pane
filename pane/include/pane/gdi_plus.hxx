#pragma once
#include <Windows.h>
#include <objbase.h>
#include <gdiplus.h>

namespace pane {
struct gdi_plus {
    using Self = gdi_plus;

    gdi_plus();
    ~gdi_plus();

    gdi_plus(Self&& color) noexcept = default;
    auto operator=(Self&& color) noexcept -> Self& = default;

    gdi_plus(const Self& color) = delete;
    auto operator=(const Self& color) -> Self& = delete;

    operator HRESULT(this Self & self);

    HRESULT result;

private:
    Gdiplus::GdiplusStartupInput startup_input;
    ULONG_PTR token;
    Gdiplus::Status status;
};
} // namespace pane
