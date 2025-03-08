#pragma once
#include <Windows.h>
#include <ShlObj.h>
#include <expected>
#include <filesystem>
#include <format>
#include <initializer_list>
#include <string>

namespace pane::filesystem {
namespace folder {
    auto app_data() -> std::expected<std::filesystem::path, std::u8string>;
    auto temp() -> std::expected<std::filesystem::path, std::u8string>;
} // namespace folder

namespace directory {
    auto from_path(const std::filesystem::path& path)
        -> std::expected<std::filesystem::path, std::u8string>;
    auto from_template(const std::filesystem::path& path,
                       const std::filesystem::path& template_directory)
        -> std::expected<std::filesystem::path, std::u8string>;
} // namespace directory

namespace file {
    auto move(const std::filesystem::path& origin, const std::filesystem::path& destination)
        -> std::expected<std::filesystem::path, std::u8string>;
    auto copy(const std::filesystem::path& origin, const std::filesystem::path& destination)
        -> std::expected<std::filesystem::path, std::u8string>;
    auto erase(const std::filesystem::path& path)
        -> std::expected<std::filesystem::path, std::u8string>;
} // namespace file

namespace symlink {
    auto create(const std::filesystem::path& target, const std::filesystem::path& destination)
        -> bool;
}
} // namespace pane::filesystem

namespace std {
template <> struct formatter<std::filesystem::path> : formatter<string_view> {
    auto format(const std::filesystem::path& path, format_context& context) const noexcept {
        return formatter<string_view>::format(path.string(), context);
    }
};

template <> struct formatter<std::filesystem::path, wchar_t> : formatter<wstring_view, wchar_t> {
    auto format(const std::filesystem::path& path, wformat_context& context) const noexcept {
        return formatter<wstring_view, wchar_t>::format(path.wstring(), context);
    }
};
} // namespace std
