#pragma once
#include <Windows.h>
#include <ShlObj.h>
#include <expected>
#include <filesystem>
#include <string>
#include <system_error>
#include <pane/string.hxx>
#include <pane/hstring.hxx>

namespace pane {
struct path {
    using Self = path;

    path() = default;
    ~path() = default;

    path(Self&& str) noexcept = default;
    auto operator=(Self&& str) noexcept -> Self& = default;

    path(const Self& str) = default;
    auto operator=(const Self& str) -> Self& = default;

    explicit path(std::filesystem::path&& path) noexcept;
    auto operator=(std::filesystem::path&& path) noexcept -> Self&;

    explicit path(const string& string) noexcept;
    auto operator=(const string& string) noexcept -> Self&;

    explicit path(const hstring& string) noexcept;
    auto operator=(const hstring& string) noexcept -> Self&;

    static auto from_known_folder(KNOWNFOLDERID known_folder = FOLDERID_LocalAppData)
        -> std::expected<Self, std::error_code>;

    static auto temp_folder() -> std::expected<Self, std::error_code>;

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
