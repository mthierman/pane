#pragma once
#include <Windows.h>
#include <string>
#include <system_error>

namespace pane {
auto get_last_error() -> std::error_code;
auto module_handle() -> HMODULE;
auto format_message(HRESULT error_code) -> std::u8string;
auto null_brush() -> HBRUSH;
auto arrow_cursor() -> HCURSOR;
auto application_icon() -> HICON;
auto resource_icon() -> HICON;
auto message_loop() -> int;
} // namespace pane
