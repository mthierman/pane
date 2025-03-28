#include <pane/gdi_plus.hxx>

namespace pane {
gdi_plus::gdi_plus() { }

gdi_plus::~gdi_plus() {
    if (SUCCEEDED(result)) {
        CoUninitialize();
    }
}
} // namespace pane
