#pragma once
#include <Windows.h>
#include <expected>
#include <system_error>
#include <pane/string.hxx>

namespace pane {
auto hresult_error(HRESULT hresult) -> std::error_code;
auto last_error() -> std::error_code;
auto module_handle() -> std::expected<HMODULE, std::error_code>;
auto format_message(HRESULT hresult) -> std::expected<string, std::error_code>;
auto null_brush() -> HBRUSH;
auto arrow_cursor() -> HCURSOR;
auto application_icon() -> HICON;
auto resource_icon() -> std::expected<HICON, std::error_code>;
auto message_loop() -> int;
} // namespace pane
