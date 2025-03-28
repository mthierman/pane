#pragma once
#include <pane/string.hxx>

namespace pane {
auto message_box(string message, string title) -> int;
auto error_box(string message, string title) -> int;
} // namespace pane
