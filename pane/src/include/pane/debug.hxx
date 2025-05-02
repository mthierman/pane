#pragma once
#include <Windows.h>
#include <format>
#include <string>
#include <pane/text.hxx>

namespace pane {
auto debug(const std::string& string) -> void;
auto debug(const std::u8string& string) -> void;
auto debug(const std::wstring& string) -> void;
auto debug(const std::u16string& string) -> void;

template <typename... Args>
auto debug(std::format_string<Args...> format_string, Args&&... args) -> void {
    if (auto converted_format_string {
            pane::to_utf16(std::vformat(format_string.get(), std::make_format_args(args...))) }) {
        OutputDebugStringW(reinterpet_cast<const wchar_t*>(converted_format_string.data()));
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
    if (auto converted_format_string {
            pane::to_utf16(std::vformat(format_string.get(), std::make_format_args(args...))) }) {
        MessageBoxW(nullptr,
                    reinterpet_cast<const wchar_t*>(converted_format_string.data()),
                    L"Info",
                    MB_OK | MB_ICONASTERISK);
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
    if (auto converted_format_string {
            pane::to_utf16(std::vformat(format_string.get(), std::make_format_args(args...))) }) {
        MessageBoxW(nullptr,
                    reinterpet_cast<const wchar_t*>(converted_format_string.data()),
                    L"Error",
                    MB_OK | MB_ICONHAND);
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
