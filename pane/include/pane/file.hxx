#pragma once
#include <Windows.h>
#include <ShlObj.h>
#include <expected>
#include <filesystem>
#include <format>
#include <initializer_list>
#include <vector>
#include <string>
#include <system_error>
#include <wil/com.h>
#include <wil/resource.h>
#include <pane/string.hxx>
#include <pane/hstring.hxx>
#include <pane/url.hxx>

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

    auto create_directory(this Self& self) -> std::expected<void, std::error_code>;
    auto create_directory_from_template(this Self& self, const Self& template_directory)
        -> std::expected<void, std::error_code>;

    auto create(this Self& self) -> bool;
    auto open(this Self& self) -> std::expected<wil::unique_handle, std::error_code>;
    auto move(this Self& self, const Self& destination) -> std::expected<void, std::error_code>;
    auto copy(this Self& self, const Self& destination) -> std::expected<void, std::error_code>;
    auto erase(this Self& self) -> std::expected<void, std::error_code>;

    auto create_symlink(this Self& self, const Self& destination)
        -> std::expected<void, std::error_code>;

    auto open_library(this Self& self)
        -> std::expected<wil::com_ptr<IShellLibrary>, std::error_code>;
    static auto library_directories(const wil::com_ptr<IShellLibrary>& lib)
        -> std::expected<std::vector<Self>, std::error_code>;

    auto download_from_url(this Self& self, url url) -> std::expected<void, std::error_code>;

    std::filesystem::path storage;
};
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
