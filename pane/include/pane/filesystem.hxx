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
#include <pane/string.hxx>
#include <pane/hstring.hxx>

namespace pane {
struct file {
    using Self = file;

    file() = default;
    ~file() = default;

    file(Self&& str) noexcept = default;
    auto operator=(Self&& str) noexcept -> Self& = default;

    file(const Self& str) = default;
    auto operator=(const Self& str) -> Self& = default;

    explicit file(std::filesystem::path&& path) noexcept;
    auto operator=(std::filesystem::path&& path) noexcept -> Self&;

    explicit file(const string& string) noexcept;
    auto operator=(const string& string) noexcept -> Self&;

    explicit file(const hstring& string) noexcept;
    auto operator=(const hstring& string) noexcept -> Self&;

    static auto from_known_folder(KNOWNFOLDERID known_folder = FOLDERID_LocalAppData)
        -> std::expected<Self, std::error_code>;
    static auto from_temp_folder() -> std::expected<Self, std::error_code>;

    auto create() -> bool;
    // auto open() -> std::expected<wil::unique_handle, std::error_code>;

    // static auto create_always(const std::filesystem::path& path)
    //     -> std::expected<Self, std::error_code>;
    // static auto create_new(const std::filesystem::path& path)
    //     -> std::expected<Self, std::error_code>;

    // static auto open_always(const std::filesystem::path& path)
    //     -> std::expected<Self, std::u8string>;
    // static auto open_existing(const std::filesystem::path& path)
    //     -> std::expected<Self, std::u8string>;

    // auto create_directory(const std::filesystem::path& path)
    //     -> std::expected<std::filesystem::path, std::u8string>;
    // auto create_directory(const std::filesystem::path& path,
    //                       const std::filesystem::path& template_directory)
    //     -> std::expected<std::filesystem::path, std::u8string>;
    // auto move_file(const std::filesystem::path& origin, const std::filesystem::path& destination)
    //     -> std::expected<std::filesystem::path, std::u8string>;
    // auto copy_file(const std::filesystem::path& origin, const std::filesystem::path& destination)
    //     -> std::expected<std::filesystem::path, std::u8string>;
    // auto erase_file(const std::filesystem::path& path)
    //     -> std::expected<std::filesystem::path, std::u8string>;
    // auto create_symlink(const std::filesystem::path& target,
    //                     const std::filesystem::path& destination)
    //     -> std::expected<std::filesystem::path, std::u8string>;

    std::filesystem::path storage;
};

struct library {
    using Self = library;

    static auto create_from_name() -> ::HRESULT;
    static auto create() -> ::IShellLibrary*;
    static auto get_folders(::IShellLibrary* lib) -> std::vector<std::u8string>;
    // static auto create() -> std::expected<Self, std::u8string>;
};

// auto download(std::u8string_view url, const std::filesystem::path& path)
//     -> std::expected<Self, std::u8string>;
} // namespace pane

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
