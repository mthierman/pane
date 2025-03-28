#pragma once
#include <Windows.h>
#include <format>
#include <pane/hstring.hxx>

// https://stackoverflow.com/questions/57547273/how-to-use-source-location-in-a-variadic-template-function

namespace pane {
template <typename... Args>
auto debug(std::format_string<Args...> format_string, Args&&... args) -> void {
    if (auto converted_format_string { pane::hstring::from_utf8(
            std::vformat(format_string.get(), std::make_format_args(args...))) }) {
        OutputDebugStringW(converted_format_string.value().c_str());
    }

    OutputDebugStringW(L"\n");
}

template <typename... Args>
auto debug(std::wformat_string<Args...> format_string, Args&&... args) -> void {
    OutputDebugStringW(std::vformat(format_string.get(), std::make_wformat_args(args...)).c_str());
    OutputDebugStringW(L"\n");
}

template <typename... Args>
auto debug_message(std::format_string<Args...> format_string, Args&&... args) -> void {
    if (auto converted_format_string { pane::hstring::from_utf8(
            std::vformat(format_string.get(), std::make_format_args(args...))) }) {
        MessageBoxW(
            nullptr, converted_format_string.value().c_str(), L"Info", MB_OK | MB_ICONASTERISK);
    }
}

template <typename... Args>
auto debug_message(std::wformat_string<Args...> format_string, Args&&... args) -> void {
    MessageBoxW(nullptr,
                std::vformat(format_string.get(), std::make_wformat_args(args...)).c_str(),
                L"Info",
                MB_OK | MB_ICONASTERISK);
}

template <typename... Args>
auto debug_error(std::format_string<Args...> format_string, Args&&... args) -> void {
    if (auto converted_format_string { pane::hstring::from_utf8(
            std::vformat(format_string.get(), std::make_format_args(args...))) }) {
        MessageBoxW(
            nullptr, converted_format_string.value().c_str(), L"Error", MB_OK | MB_ICONHAND);
    }
}

template <typename... Args>
auto debug_error(std::wformat_string<Args...> format_string, Args&&... args) -> void {
    MessageBoxW(nullptr,
                std::vformat(format_string.get(), std::make_wformat_args(args...)).c_str(),
                L"Error",
                MB_OK | MB_ICONHAND);
}
} // namespace pane
