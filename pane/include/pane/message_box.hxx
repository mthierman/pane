#pragma once
#include <string>

namespace pane {
auto message_box(std::u8string message, std::u8string title) -> int;
auto error_box(std::u8string message, std::u8string title) -> int;
} // namespace pane
