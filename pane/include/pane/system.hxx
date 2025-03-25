#pragma once
#include <Windows.h>
#include <optional>
#include <string>

namespace pane {
auto module_handle() -> HMODULE;
auto format_message(HRESULT error_code) -> std::u8string;
auto last_error() -> std::u8string;
auto null_brush() -> HBRUSH;
auto arrow_cursor() -> HCURSOR;
auto application_icon() -> HICON;
auto resource_icon() -> HICON;

auto message_loop() -> int;
} // namespace pane
