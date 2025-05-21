#pragma once
#include <pane/pane.hxx>

namespace pane::example {
struct window_manager {
    using Self = window_manager;

    window_manager(pane::window_config&& window_config = {});
    ~window_manager() = default;
};
} // namespace pane::example
