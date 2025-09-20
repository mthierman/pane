#include <pane/co_init.hpp>

namespace pane {
co_init::co_init(DWORD concurrency_model)
    : result { CoInitializeEx(nullptr, concurrency_model) } { }

co_init::~co_init() {
    if (SUCCEEDED(result)) {
        CoUninitialize();
    }
}

co_init::operator HRESULT(this Self& self) { return self.result; }

auto co_init::apartment_threaded() -> co_init {
    return Self(COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
}

auto co_init::multi_threaded() -> co_init {
    return Self(COINIT_MULTITHREADED | COINIT_DISABLE_OLE1DDE);
}
} // namespace pane
