#pragma once
#include <pane/pane.hxx>

namespace pane::example {
struct window_manager final {
    using Self = window_manager;

    auto insert(this Self& self, pane::window_handle& window_handle) -> void;
    auto erase(this Self& self, pane::window_handle& window_handle) -> void;
    auto clear(this Self& self) -> void;
    auto size(this const Self& self) -> uint64_t;
    auto contains(this const Self& self, pane::window_handle& window_handle) -> bool;
    auto empty(this const Self& self) -> bool;
    auto first(this const Self& self) -> pane::window_handle*;
    auto last(this const Self& self) -> pane::window_handle*;

private:
    std::set<pane::window_handle*> set;
};
} // namespace pane::example
