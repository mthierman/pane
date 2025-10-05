#pragma once
#include <Windows.h>
#include <pane/text.hpp>

namespace pane {
template <typename... Args>
auto message_box(std::format_string<Args...> format_string, Args&&... args) -> void {
    auto msg { to_utf16_lossy(std::format(format_string, std::forward<Args>(args)...)) };
    MessageBoxW(nullptr, as_c_str(msg), L"Info", MB_OK | MB_ICONASTERISK);
}

template <typename... Args>
auto message_box(std::wformat_string<Args...> format_string, Args&&... args) -> void {
    auto msg { std::format(format_string, std::forward<Args>(args)...) };
    MessageBoxW(nullptr, as_c_str(msg), L"Info", MB_OK | MB_ICONASTERISK);
}

template <typename... Args>
auto error_box(std::format_string<Args...> format_string, Args&&... args) -> void {
    auto msg { to_utf16_lossy(std::format(format_string, std::forward<Args>(args)...)) };
    MessageBoxW(nullptr, as_c_str(msg), L"Info", MB_OK | MB_ICONHAND);
}

template <typename... Args>
auto error_box(std::wformat_string<Args...> format_string, Args&&... args) -> void {
    auto msg { std::format(format_string, std::forward<Args>(args)...) };
    MessageBoxW(nullptr, as_c_str(msg), L"Info", MB_OK | MB_ICONHAND);
}

template <typename... Args>
auto debug_print(std::format_string<Args...> format_string, Args&&... args) -> void {
    auto msg { to_utf16_lossy(std::format(format_string, std::forward<Args>(args)...)) };
    OutputDebugStringW(as_c_str(msg.data()));
}

template <typename... Args>
auto debug_println(std::format_string<Args...> format_string, Args&&... args) -> void {
    auto msg { to_utf16_lossy(std::format(format_string, std::forward<Args>(args)...)) };
    OutputDebugStringW(as_c_str(msg.data()));
    OutputDebugStringW(L"\n");
}

template <typename... Args>
auto debug_print(std::wformat_string<Args...> format_string, Args&&... args) -> void {
    auto msg { std::format(format_string, std::forward<Args>(args)...) };
    OutputDebugStringW(as_c_str(msg.data()));
}

template <typename... Args>
auto debug_println(std::wformat_string<Args...> format_string, Args&&... args) -> void {
    auto msg { std::format(format_string, std::forward<Args>(args)...) };
    OutputDebugStringW(as_c_str(msg.data()));
    OutputDebugStringW(L"\n");
}
} // namespace pane
