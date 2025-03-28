#pragma once
#include <Windows.h>
#include <format>
#include <string>
#include <pane/string.hxx>

namespace pane {
auto message_box(string message, string title) -> void;
// auto error_box(std::u8string message, std::u8string title) -> void;
} // namespace pane
