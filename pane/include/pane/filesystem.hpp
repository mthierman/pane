#pragma once
#include <Windows.h>
#include <ShlObj.h>
#include <ShObjIdl.h>
#include <KnownFolders.h>
#include <expected>
#include <filesystem>
#include <format>
#include <vector>
#include <string>
#include <system_error>
#include <wil/com.h>
#include <wil/resource.h>
#include <ada.h>

namespace pane::filesystem {
auto known_folder(KNOWNFOLDERID known_folder = FOLDERID_LocalAppData)
    -> std::expected<std::filesystem::path, HRESULT>;
auto temp_folder() -> std::expected<std::filesystem::path, HRESULT>;

auto create_directory(const std::filesystem::path& path) -> std::expected<void, HRESULT>;
auto create_directory_from_template(const std::filesystem::path& path,
                                    const std::filesystem::path& template_directory)
    -> std::expected<void, HRESULT>;

auto create_file(const std::filesystem::path& path) -> std::expected<void, HRESULT>;
auto open_file(const std::filesystem::path& path) -> std::expected<wil::unique_handle, HRESULT>;
auto move_file(const std::filesystem::path& path, const std::filesystem::path& destination)
    -> std::expected<void, HRESULT>;
auto copy_file(const std::filesystem::path& path, const std::filesystem::path& destination)
    -> std::expected<void, HRESULT>;
auto erase_file(const std::filesystem::path& path) -> std::expected<void, HRESULT>;
auto download_file(const std::filesystem::path& path, ada::url url) -> std::expected<void, HRESULT>;

auto create_symlink(const std::filesystem::path& path, const std::filesystem::path& destination)
    -> std::expected<void, HRESULT>;

auto open_library(const std::filesystem::path& path)
    -> std::expected<wil::com_ptr<IShellLibrary>, HRESULT>;
auto open_library(const wil::com_ptr<IShellItem>& item)
    -> std::expected<wil::com_ptr<IShellLibrary>, HRESULT>;
auto library_directories(const wil::com_ptr<IShellLibrary>& lib)
    -> std::expected<std::vector<std::filesystem::path>, HRESULT>;
auto get_path(const wil::com_ptr<IShellItem>& item) -> std::expected<std::u8string, HRESULT>;
} // namespace pane::filesystem

namespace std {
template <> struct formatter<std::filesystem::path> : formatter<string_view> {
    auto format(const std::filesystem::path& path, format_context& context) const noexcept {
        auto u8path { path.u8string() };

        return formatter<string_view>::format(
            { reinterpret_cast<const char*>(u8path.data()), u8path.length() }, context);
    }
};

template <> struct formatter<std::filesystem::path, wchar_t> : formatter<wstring_view, wchar_t> {
    auto format(const std::filesystem::path& path, wformat_context& context) const noexcept {
        return formatter<wstring_view, wchar_t>::format(path.c_str(), context);
    }
};
} // namespace std
