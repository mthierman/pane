#pragma once
#include <Windows.h>
#include <format>
#include <pane/hstring.hxx>

namespace pane {
template <typename... Args>
auto message_box(std::format_string<Args...> format_string, Args&&... args) -> void {
    if (auto converted_format_string { hstring::from_utf8(
            std::vformat(format_string.get(), std::make_format_args(args...))) }) {
        MessageBoxW(
            nullptr, converted_format_string.value().c_str(), L"Info", MB_OK | MB_ICONASTERISK);
    }
}

template <typename... Args>
auto message_box(std::wformat_string<Args...> format_string, Args&&... args) -> void {
    MessageBoxW(nullptr,
                std::vformat(format_string.get(), std::make_wformat_args(args...)).c_str(),
                L"Info",
                MB_OK | MB_ICONASTERISK);
}

template <typename... Args>
auto error_box(std::format_string<Args...> format_string, Args&&... args) -> void {
    if (auto converted_format_string { hstring::from_utf8(
            std::vformat(format_string.get(), std::make_format_args(args...))) }) {
        MessageBoxW(
            nullptr, converted_format_string.value().c_str(), L"Error", MB_OK | MB_ICONHAND);
    }
}

template <typename... Args>
auto error_box(std::wformat_string<Args...> format_string, Args&&... args) -> void {
    MessageBoxW(nullptr,
                std::vformat(format_string.get(), std::make_wformat_args(args...)).c_str(),
                L"Error",
                MB_OK | MB_ICONHAND);
}
} // namespace pane
