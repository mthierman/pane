#pragma once
#include <string_view>
#include <string>

namespace pane {
auto to_utf16(std::u8string_view string) -> std::u16string;
auto to_utf16(std::string_view string) -> std::u16string;

auto to_utf8(std::u16string_view string) -> std::u8string;
auto to_utf8(std::wstring_view string) -> std::u8string;
} // namespace pane
