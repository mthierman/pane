#include <pane/filesystem.hxx>
#include <pane/system.hxx>
#include <wil/resource.h>

namespace pane::filesystem {
namespace paths {
    auto app_data() -> std::expected<std::filesystem::path, std::u8string> {
        wil::unique_cotaskmem_string buffer;

        if (auto result { ::SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, nullptr, &buffer) };
            result == S_OK) {
            return buffer.get();
        } else {
            return std::unexpected(pane::sys::format_message(result));
        }
    }

    auto temp() -> std::expected<std::filesystem::path, std::u8string> {
        std::wstring buffer;
        buffer.resize(::GetTempPathW(0, buffer.data()));

        if (auto result { ::GetTempPathW(static_cast<::DWORD>(buffer.size()), buffer.data()) };
            result != 0) {
            buffer.resize(buffer.size() - 2);

            return buffer;
        } else {
            return std::unexpected(pane::sys::last_error());
        }
    }
} // namespace paths

namespace directory {
    auto from_path(const std::filesystem::path& path)
        -> std::expected<std::filesystem::path, std::u8string> {
        if (auto result { ::CreateDirectoryW(path.c_str(), nullptr) }; result != 0) {
            return path;
        } else {
            return std::unexpected(pane::sys::last_error());
        }
    }

    auto from_template(const std::filesystem::path& path,
                       const std::filesystem::path& template_directory)
        -> std::expected<std::filesystem::path, std::u8string> {
        if (auto result { ::CreateDirectoryExW(template_directory.c_str(), path.c_str(), nullptr) };
            result != 0) {
            return path;
        } else {
            return std::unexpected(pane::sys::last_error());
        }
    }
} // namespace directory

namespace file {
    auto move(const std::filesystem::path& origin, const std::filesystem::path& destination)
        -> std::expected<std::filesystem::path, std::u8string> {
        if (auto result { ::MoveFileW(origin.c_str(), destination.c_str()) }; result != 0) {
            return destination;
        } else {
            return std::unexpected(pane::sys::last_error());
        }
    }

    auto copy(const std::filesystem::path& origin, const std::filesystem::path& destination)
        -> bool {
        if (auto result { ::CopyFile2(origin.c_str(), destination.c_str(), nullptr) };
            result == S_OK) {
            return true;
        }

        return false;
    }

    auto erase(const std::filesystem::path& path) -> bool {
        if (auto result { ::DeleteFileW(path.c_str()) }; result != 0) {
            return true;
        }

        return false;
    }
} // namespace file

namespace symlink {
    auto create(const std::filesystem::path& target, const std::filesystem::path& destination)
        -> bool {
        auto flags { std::filesystem::is_directory(target)
                         ? SYMBOLIC_LINK_FLAG_DIRECTORY
                             | SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE
                         : SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE };

        if (auto result { ::CreateSymbolicLinkW(
                destination.c_str(), target.c_str(), static_cast<::DWORD>(flags)) };
            result != 0) {
            return true;
        }

        return false;
    }
} // namespace symlink
} // namespace pane::filesystem
