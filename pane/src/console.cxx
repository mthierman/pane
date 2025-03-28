#include <pane/console.hxx>
#include <iostream>

namespace pane {
console::console() {
    AllocConsole();

    freopen_s(file.addressof(), "CONIN$", "r", stdin);
    freopen_s(file.addressof(), "CONOUT$", "w", stdout);
    freopen_s(file.addressof(), "CONIN$", "w", stderr);

    std::cin.clear();
    std::cout.clear();
    std::cerr.clear();
    std::clog.clear();
}

console::~console() { FreeConsole(); }
} // namespace pane
