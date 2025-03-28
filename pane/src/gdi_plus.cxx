#include <pane/gdi_plus.hxx>

namespace pane {
gdi_plus::gdi_plus()
    : status { Gdiplus::GdiplusStartup(&token, &startup_input, nullptr) } { }

gdi_plus::~gdi_plus() {
    if (status == Gdiplus::Status::Ok) {
        Gdiplus::GdiplusShutdown(token);
    }
}
} // namespace pane
