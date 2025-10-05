#pragma once
#include <Windows.h>
// #include <format>
// #include <string>
#include <pane/text.hpp>

namespace pane {
template <typename... Args>
auto debug_print(std::format_string<Args...> format_string, Args&&... args) -> void {
    auto msg { to_utf16_lossy(std::format(format_string, std::forward<Args>(args)...)) };
    OutputDebugStringW(as_c_str(msg.data()));
}

template <typename... Args>
auto debug_println(std::format_string<Args...> format_string, Args&&... args) -> void {
    debug_print("{}\n", std::format(format_string, std::forward<Args>(args)...));
}

// auto debug(const std::string& string) -> void;
// auto debug(const std::u8string& string) -> void;
// auto debug(const std::wstring& string) -> void;
// auto debug(const std::u16string& string) -> void;

// template <typename... Args>
// auto debug(std::format_string<Args...> format_string, Args&&... args) -> void {
//     const auto u16string { pane::to_utf16(
//         std::vformat(format_string.get(), std::make_format_args(args...))) };

//     OutputDebugStringW(reinterpret_cast<const wchar_t*>(u16string.data()));
//     OutputDebugStringW(L"\n");
// }

// template <typename... Args>
// auto debug(std::wformat_string<Args...> format_string, Args&&... args) -> void {
//     OutputDebugStringW(std::vformat(format_string.get(),
//     std::make_wformat_args(args...)).data()); OutputDebugStringW(L"\n");
// }

// template <typename... Args>
// auto debug_message(std::format_string<Args...> format_string, Args&&... args) -> void {
//     const auto u16string { pane::to_utf16(
//         std::vformat(format_string.get(), std::make_format_args(args...))) };

//     MessageBoxW(nullptr,
//                 reinterpet_cast<const wchar_t*>(u16string.data()),
//                 L"Info",
//                 MB_OK | MB_ICONASTERISK);
// }

// template <typename... Args>
// auto debug_message(std::wformat_string<Args...> format_string, Args&&... args) -> void {
//     MessageBoxW(nullptr,
//                 std::vformat(format_string.get(), std::make_wformat_args(args...)).data(),
//                 L"Info",
//                 MB_OK | MB_ICONASTERISK);
// }

// template <typename... Args>
// auto debug_error(std::format_string<Args...> format_string, Args&&... args) -> void {
//     const auto u16string { pane::to_utf16(
//         std::vformat(format_string.get(), std::make_format_args(args...))) };

//     MessageBoxW(
//         nullptr, reinterpet_cast<const wchar_t*>(u16string.data()), L"Error", MB_OK |
//         MB_ICONHAND);
// }

// template <typename... Args>
// auto debug_error(std::wformat_string<Args...> format_string, Args&&... args) -> void {
//     MessageBoxW(nullptr,
//                 std::vformat(format_string.get(), std::make_wformat_args(args...)).data(),
//                 L"Error",
//                 MB_OK | MB_ICONHAND);
// }
} // namespace pane
