#pragma once
#include <Windows.h>
#include <expected>
#include <string>
#include <system_error>
#include <pane/string.hxx>

namespace pane {
auto get_last_error() -> std::error_code;
auto module_handle() -> std::expected<HMODULE, std::error_code>;
auto format_message(HRESULT error_code) -> std::expected<string, std::error_code>;
auto null_brush() -> HBRUSH;
auto arrow_cursor() -> HCURSOR;
auto application_icon() -> HICON;
auto resource_icon() -> HICON;
auto message_loop() -> int;
} // namespace pane
