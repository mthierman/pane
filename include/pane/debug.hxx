#pragma once
#include <Windows.h>
#include <format>
#include <string>
#include <pane/text.hxx>

namespace pane {
#ifdef DEBUG_MODE
constexpr bool debug_mode { true };
#else
constexpr bool debug_mode { false };
#endif

auto debug(const std::string& string) -> void;
auto debug(const std::u8string& string) -> void;
auto debug(const std::wstring& string) -> void;
auto debug(const std::u16string& string) -> void;

template <typename... Args>
auto debug(std::format_string<Args...> format_string, Args&&... args) -> void {
    const auto u16string { pane::to_utf16(
        std::vformat(format_string.get(), std::make_format_args(args...))) };

    OutputDebugStringW(reinterpret_cast<const wchar_t*>(u16string.data()));
    OutputDebugStringW(L"\n");
}

template <typename... Args>
auto debug(std::wformat_string<Args...> format_string, Args&&... args) -> void {
    OutputDebugStringW(std::vformat(format_string.get(), std::make_wformat_args(args...)).data());
    OutputDebugStringW(L"\n");
}

template <typename... Args>
auto debug_message(std::format_string<Args...> format_string, Args&&... args) -> void {
    const auto u16string { pane::to_utf16(
        std::vformat(format_string.get(), std::make_format_args(args...))) };

    MessageBoxW(nullptr,
                reinterpet_cast<const wchar_t*>(u16string.data()),
                L"Info",
                MB_OK | MB_ICONASTERISK);
}

template <typename... Args>
auto debug_message(std::wformat_string<Args...> format_string, Args&&... args) -> void {
    MessageBoxW(nullptr,
                std::vformat(format_string.get(), std::make_wformat_args(args...)).data(),
                L"Info",
                MB_OK | MB_ICONASTERISK);
}

template <typename... Args>
auto debug_error(std::format_string<Args...> format_string, Args&&... args) -> void {
    const auto u16string { pane::to_utf16(
        std::vformat(format_string.get(), std::make_format_args(args...))) };

    MessageBoxW(
        nullptr, reinterpet_cast<const wchar_t*>(u16string.data()), L"Error", MB_OK | MB_ICONHAND);
}

template <typename... Args>
auto debug_error(std::wformat_string<Args...> format_string, Args&&... args) -> void {
    MessageBoxW(nullptr,
                std::vformat(format_string.get(), std::make_wformat_args(args...)).data(),
                L"Error",
                MB_OK | MB_ICONHAND);
}
} // namespace pane
