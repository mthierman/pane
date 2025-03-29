#include <pane/filesystem.hxx>
#include <pane/system.hxx>
#include <pane/text.hxx>
#include <urlmon.h>

namespace pane::filesystem {
auto known_folder(KNOWNFOLDERID known_folder)
    -> std::expected<std::filesystem::path, std::error_code> {
    wil::unique_cotaskmem_string buffer;

    if (auto result { SHGetKnownFolderPath(known_folder, KF_FLAG_DONT_VERIFY, nullptr, &buffer) };
        FAILED(result)) {
        return std::unexpected(pane::system::hresult_error(result));
    }

    return buffer.get();
}

auto temp_folder() -> std::expected<std::filesystem::path, std::error_code> {
    std::wstring buffer;

    auto length { GetTempPath2W(0, buffer.data()) };

    if (length == 0) {
        return std::unexpected(pane::system::last_error());
    }

    buffer.resize(length);

    if (GetTempPath2W(length, buffer.data()) == 0) {
        return std::unexpected(pane::system::last_error());
    }

    buffer.resize(buffer.size() - 2);

    return buffer;
}

auto create_directory(const std::filesystem::path& path) -> std::expected<void, std::error_code> {
    if (CreateDirectoryW(path.c_str(), nullptr) == 0) {
        return std::unexpected(pane::system::last_error());
    }

    return {};
}

auto create_directory_from_template(const std::filesystem::path& path,
                                    const std::filesystem::path& template_directory)
    -> std::expected<void, std::error_code> {
    if (CreateDirectoryExW(template_directory.c_str(), path.c_str(), nullptr) == 0) {
        return std::unexpected(pane::system::last_error());
    }

    return {};
}

auto create_file(const std::filesystem::path& path) -> bool {
    if (CreateFile2(path.c_str(), 0, 0, CREATE_NEW, nullptr) == INVALID_HANDLE_VALUE) {
        return false;
    }

    return true;
}

auto open_file(const std::filesystem::path& path)
    -> std::expected<wil::unique_handle, std::error_code> {
    auto handle { wil::unique_handle(
        CreateFile2(path.c_str(), GENERIC_READ | GENERIC_WRITE, 0, OPEN_EXISTING, nullptr)) };

    if (handle.get() == INVALID_HANDLE_VALUE) {
        return std::unexpected(pane::system::last_error());
    }

    return handle;
}

auto move_file(const std::filesystem::path& origin, const std::filesystem::path& destination)
    -> std::expected<void, std::error_code> {
    if (MoveFileW(origin.c_str(), destination.c_str()) == 0) {
        return std::unexpected(pane::system::last_error());
    }

    return {};
}

auto copy_file(const std::filesystem::path& origin, const std::filesystem::path& destination)
    -> std::expected<void, std::error_code> {
    if (auto result { CopyFile2(origin.c_str(), destination.c_str(), nullptr) }; FAILED(result)) {
        return std::unexpected(pane::system::hresult_error(result));
    }

    return {};
}

auto erase_file(const std::filesystem::path& path) -> std::expected<void, std::error_code> {
    if (DeleteFileW(path.c_str()) == 0) {
        return std::unexpected(pane::system::last_error());
    }

    return {};
}

auto download_file(const std::filesystem::path& path, ada::url url)
    -> std::expected<void, std::error_code> {
    auto converted_url { pane::to_utf16(url.get_href()) };

    if (auto result { URLDownloadToFileW(nullptr,
                                         reinterpret_cast<const wchar_t*>(converted_url.c_str()),
                                         path.c_str(),
                                         0,
                                         nullptr) };
        FAILED(result)) {
        return std::unexpected(pane::system::hresult_error(result));
    }

    return {};
}

auto create_symlink(const std::filesystem::path& target, const std::filesystem::path& destination)
    -> std::expected<void, std::error_code> {
    auto flags { std::filesystem::is_directory(target)
                     ? SYMBOLIC_LINK_FLAG_DIRECTORY | SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE
                     : SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE };

    if (CreateSymbolicLinkW(destination.c_str(), target.c_str(), static_cast<::DWORD>(flags))
        == 0) {
        return std::unexpected(pane::system::last_error());
    }

    return {};
}

auto open_library(const std::filesystem::path& path)
    -> std::expected<wil::com_ptr<IShellLibrary>, std::error_code> {
    wil::com_ptr<IShellLibrary> lib;

    if (auto result {
            SHLoadLibraryFromParsingName(path.c_str(), STGM_READWRITE, IID_PPV_ARGS(&lib)) };
        result != S_OK) {
        return std::unexpected(pane::system::hresult_error(result));
    }

    return lib;
}

auto open_library(const wil::com_ptr<IShellItem>& item)
    -> std::expected<wil::com_ptr<IShellLibrary>, std::error_code> {
    wil::com_ptr<IShellLibrary> lib;

    if (auto result { SHLoadLibraryFromItem(item.get(), STGM_READWRITE, IID_PPV_ARGS(&lib)) };
        result != S_OK) {
        return std::unexpected(pane::system::hresult_error(result));
    }

    return lib;
}

auto library_directories(const wil::com_ptr<IShellLibrary>& lib)
    -> std::expected<std::vector<std::filesystem::path>, std::error_code> {
    wil::com_ptr<IShellItemArray> array;

    if (auto result { lib->GetFolders(LFF_ALLITEMS, IID_PPV_ARGS(&array)) }; FAILED(result)) {
        return std::unexpected(pane::system::hresult_error(result));
    }

    DWORD count;
    array->GetCount(&count);

    std::vector<std::filesystem::path> files;
    files.resize(count);

    for (DWORD i = 0; i < count; ++i) {
        wil::com_ptr<IShellItem> item;

        if (auto result { array->GetItemAt(i, &item) }; result != S_OK) {
            return std::unexpected(pane::system::hresult_error(result));
        }

        wil::unique_cotaskmem_string path;
        if (auto result { item->GetDisplayName(SIGDN_FILESYSPATH, &path) }; result != S_OK) {
            return std::unexpected(pane::system::hresult_error(result));
        }

        files[i] = path.get();
    }

    return files;
}

auto get_path(const wil::com_ptr<IShellItem>& item)
    -> std::expected<std::u8string, std::error_code> {
    SFGAOF attributes;

    if (auto result { item->GetAttributes(SFGAO_FILESYSTEM, &attributes) }; result != S_OK) {
        return std::unexpected(pane::system::hresult_error(result));
    }

    wil::unique_cotaskmem_string buffer;

    if (attributes) {
        item->GetDisplayName(SIGDN_FILESYSPATH, &buffer);
    } else {
        item->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &buffer);
    }

    return pane::to_utf8(buffer.get());
}
} // namespace pane::filesystem
