#pragma once
#include <Windows.h>
#include <ShlObj.h>
#include <expected>
#include <filesystem>
#include <format>
#include <initializer_list>
#include <string>
#include <wil/resource.h>

namespace pane::fs {
struct file {
    using Self = file;

    static auto create_always(const std::filesystem::path& path)
        -> std::expected<Self, std::u8string>;
    static auto create_new(const std::filesystem::path& path) -> std::expected<Self, std::u8string>;

    static auto open_always(const std::filesystem::path& path)
        -> std::expected<Self, std::u8string>;
    static auto open_existing(const std::filesystem::path& path)
        -> std::expected<Self, std::u8string>;

    static auto from_url(std::u8string_view url, const std::filesystem::path& path)
        -> std::expected<Self, std::u8string>;

    wil::unique_handle handle;
};

// struct library {
//     using Self = library;

//     static auto create_from_name() -> ::HRESULT;
//     // static auto create() -> std::expected<Self, std::u8string>;
// };

auto app_data() -> std::expected<std::filesystem::path, std::u8string>;
auto temp() -> std::expected<std::filesystem::path, std::u8string>;
auto create_directory(const std::filesystem::path& path)
    -> std::expected<std::filesystem::path, std::u8string>;
auto create_directory(const std::filesystem::path& path,
                      const std::filesystem::path& template_directory)
    -> std::expected<std::filesystem::path, std::u8string>;
auto move_file(const std::filesystem::path& origin, const std::filesystem::path& destination)
    -> std::expected<std::filesystem::path, std::u8string>;
auto copy_file(const std::filesystem::path& origin, const std::filesystem::path& destination)
    -> std::expected<std::filesystem::path, std::u8string>;
auto erase_file(const std::filesystem::path& path)
    -> std::expected<std::filesystem::path, std::u8string>;
auto create_symlink(const std::filesystem::path& target, const std::filesystem::path& destination)
    -> std::expected<std::filesystem::path, std::u8string>;
} // namespace pane::fs

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
