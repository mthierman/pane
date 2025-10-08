#pragma once
#include <Windows.h>
#include <cstdlib>
#include <expected>
#include <optional>
#include <string>
#include <string_view>
#include <system_error>
#include <vector>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.ViewManagement.h>
#include <pane/text.hpp>

namespace pane::system {
auto hresult_error(HRESULT hresult) -> std::error_code;
auto last_error() -> std::error_code;
auto module_handle() -> std::expected<HMODULE, HRESULT>;
auto format_message(HRESULT hresult) -> std::u8string;
auto null_brush() -> HBRUSH;
auto arrow_cursor() -> HCURSOR;
auto application_icon() -> HICON;
auto resource_icon() -> std::expected<HICON, HRESULT>;
auto ui_settings() -> winrt::Windows::UI::ViewManagement::UISettings;
auto dark_mode() -> bool;
auto get_argv() -> std::vector<std::u16string>;
auto get_argv_u8(std::span<const std::u16string> argv) -> std::vector<std::u8string>;
auto get_environment_variable(std::u8string_view name) -> std::optional<std::u8string>;
auto exit_process(unsigned int exit_code = EXIT_SUCCESS) -> void;
auto quit(int exit_code = EXIT_SUCCESS) -> void;
auto message_loop() -> int;
} // namespace pane::system

namespace std {
// template <> struct formatter<std::error_code> : formatter<string_view> {
//     auto format(const std::error_code& error_code, format_context& context) const noexcept {
//         return formatter<string_view>::format(error_code.message(), context);
//     }
// };

// template <> struct formatter<std::error_code, wchar_t> : formatter<wstring_view, wchar_t> {
//     auto format(const std::error_code& error_code, wformat_context& context) const {
//         auto converted_message { pane::to_utf16_lossy(error_code.message()) };

//         return formatter<wstring_view, wchar_t>::format(
//             { reinterpret_cast<const wchar_t*>(converted_message.data()),
//               converted_message.length() },
//             context);
//     }
// };
} // namespace std
