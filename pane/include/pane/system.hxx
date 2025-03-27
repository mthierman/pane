#pragma once
#include <Windows.h>
#include <expected>
#include <system_error>
#include <pane/string.hxx>
#include <wil/com.h>

namespace pane {
auto co_init_apartment() -> wil::unique_couninitialize_call;
auto co_init_multi() -> wil::unique_couninitialize_call;
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

namespace std {
template <> struct formatter<std::error_code> : formatter<string_view> {
    auto format(const std::error_code& error_code, format_context& context) const noexcept {
        return formatter<string_view>::format(error_code.message(), context);
    }
};
} // namespace std
