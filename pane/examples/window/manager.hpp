#pragma once
#include <pane/pane.hxx>

namespace pane::example {
struct window_manager final {
    using Self = window_manager;

    window_manager(pane::window_config&& window_config = {});
    ~window_manager() = default;

    auto insert(this Self& self, const pane::window_handle& window_handle) -> void;
    auto erase(this Self& self, const pane::window_handle& window_handle) -> void;
    auto clear(this Self& self) -> void;
    auto size(this const Self& self) -> uint64_t;
    auto contains(this const Self& self, HWND hwnd) -> bool;
    auto empty(this const Self& self) -> bool;
    auto first(this const Self& self) -> HWND;
    auto last(this const Self& self) -> HWND;

private:
    std::set<HWND> set;
};
} // namespace pane::example
