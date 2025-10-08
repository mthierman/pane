#pragma once
#include <string>
#include <vector>
#include <pane/system.hpp>

namespace pane {
struct argv final {
    auto u8string() -> std::vector<std::u8string>;

    std::vector<std::u16string> argv { system::get_argv() };
};
} // namespace pane
