#pragma once
#include <string>

namespace pane::message_box {
auto info(std::u8string message, std::u8string title = u8"Info") -> int;
auto error(std::u8string message, std::u8string title = u8"Error") -> int;
} // namespace pane::message_box
