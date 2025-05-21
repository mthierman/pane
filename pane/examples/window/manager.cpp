#include "manager.hpp"

namespace pane::example {
window_manager::window_manager(pane::window_config&& window_config) { }

auto window_manager::insert(this Self& self, const pane::window_handle& window_handle) -> void {
    self.set.insert(window_handle());
}

auto window_manager::erase(this Self& self, const pane::window_handle& window_handle) -> void {
    self.set.erase(window_handle());

    if (self.set.empty()) {
        PostQuitMessage(0);
    }
}

auto window_manager::clear(this Self& self) -> void {
    self.set.clear();

    if (self.set.empty()) {
        PostQuitMessage(0);
    }
}

auto window_manager::size(this const Self& self) -> uint64_t { return self.set.size(); }

auto window_manager::contains(this const Self& self, HWND hwnd) -> bool {
    return self.set.contains(hwnd);
}

auto window_manager::empty(this const Self& self) -> bool { return self.set.empty(); }

auto window_manager::first(this const Self& self) -> HWND { return *self.set.begin(); }

auto window_manager::last(this const Self& self) -> HWND { return *self.set.end(); }
} // namespace pane::example
