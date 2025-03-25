#pragma once
#include <Windows.h>
#include <ShlObj.h>
#include <expected>
#include <filesystem>
#include <format>
#include <initializer_list>
#include <string>
#include <system_error>
#include <wil/resource.h>

namespace pane {
// struct file {
//     using Self = file;

//     static auto create_always(const std::filesystem::path& path)
//         -> std::expected<Self, std::u8string>;
//     static auto create_new(const std::filesystem::path& path) -> std::expected<Self,
//     std::u8string>;

//     static auto open_always(const std::filesystem::path& path)
//         -> std::expected<Self, std::u8string>;
//     static auto open_existing(const std::filesystem::path& path)
//         -> std::expected<Self, std::u8string>;

//     static auto from_url(std::u8string_view url, const std::filesystem::path& path)
//         -> std::expected<Self, std::u8string>;

//     wil::unique_handle handle;
// };

struct library {
    using Self = library;

    static auto create_from_name() -> ::HRESULT;
    static auto create() -> ::IShellLibrary*;
    static auto get_folders(::IShellLibrary* lib) -> std::vector<std::u8string>;
    // static auto create() -> std::expected<Self, std::u8string>;
};

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
} // namespace pane
