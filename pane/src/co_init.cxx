#include <pane/co_init.hxx>

namespace pane {
co_init::~co_init() {
    if (SUCCEEDED(result)) {
        CoUninitialize();
    }
}

auto co_init::apartment_threaded() -> Self {
    Self self;
    self.result = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

    return self;
}

auto co_init::multi_threaded() -> Self {
    Self self;
    self.result = CoInitializeEx(nullptr, COINIT_MULTITHREADED | COINIT_DISABLE_OLE1DDE);

    return self;
}
} // namespace pane
