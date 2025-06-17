#include <pane/filesystem.hxx>
#include <pane/system.hxx>
#include <pane/text.hxx>
#include <urlmon.h>

namespace pane::filesystem {
auto known_folder(KNOWNFOLDERID known_folder) -> std::expected<std::filesystem::path, HRESULT> {
    wil::unique_cotaskmem_string buffer;

    if (auto hresult { SHGetKnownFolderPath(known_folder, KF_FLAG_DONT_VERIFY, nullptr, &buffer) };
        hresult != S_OK) {
        return std::unexpected(hresult);
    }

    return buffer.get();
}

auto temp_folder() -> std::expected<std::filesystem::path, HRESULT> {
    std::wstring buffer;

    if (auto length { GetTempPath2W(0, buffer.data()) }; length == 0) {
        auto last_error { GetLastError() };
        return std::unexpected(HRESULT_FROM_WIN32(last_error));
    } else {
        buffer.resize(length);
    }

    if (auto length { GetTempPath2W(static_cast<DWORD>(buffer.length()), buffer.data()) };
        length == 0) {
        auto last_error { GetLastError() };
        return std::unexpected(HRESULT_FROM_WIN32(last_error));
    } else {
        buffer.resize(buffer.size() - 2);
    }

    return buffer;
}

auto create_directory(const std::filesystem::path& path) -> std::expected<void, HRESULT> {
    if (auto result { CreateDirectoryW(path.c_str(), nullptr) }; result == 0) {
        auto last_error { GetLastError() };
        return std::unexpected(HRESULT_FROM_WIN32(last_error));
    }

    return {};
}

auto create_directory_from_template(const std::filesystem::path& path,
                                    const std::filesystem::path& template_directory)
    -> std::expected<void, HRESULT> {
    if (auto result { CreateDirectoryExW(template_directory.c_str(), path.c_str(), nullptr) };
        result == 0) {
        auto last_error { GetLastError() };
        return std::unexpected(HRESULT_FROM_WIN32(last_error));
    }

    return {};
}

auto create_file(const std::filesystem::path& path) -> std::expected<void, HRESULT> {
    if (auto handle { CreateFile2(path.c_str(), 0, 0, CREATE_NEW, nullptr) };
        handle == INVALID_HANDLE_VALUE) {
        auto last_error { GetLastError() };
        return std::unexpected(HRESULT_FROM_WIN32(last_error));
    }

    return {};
}

auto open_file(const std::filesystem::path& path) -> std::expected<wil::unique_handle, HRESULT> {
    auto handle { CreateFile2(
        path.c_str(), GENERIC_READ | GENERIC_WRITE, 0, OPEN_EXISTING, nullptr) };

    if (handle == INVALID_HANDLE_VALUE) {
        auto last_error { GetLastError() };
        return std::unexpected(HRESULT_FROM_WIN32(last_error));
    }

    return wil::unique_handle(handle);
}

auto move_file(const std::filesystem::path& origin, const std::filesystem::path& destination)
    -> std::expected<void, HRESULT> {
    if (auto result { MoveFileW(origin.c_str(), destination.c_str()) }; result == 0) {
        auto last_error { GetLastError() };
        return std::unexpected(HRESULT_FROM_WIN32(last_error));
    }

    return {};
}

auto copy_file(const std::filesystem::path& origin, const std::filesystem::path& destination)
    -> std::expected<void, HRESULT> {
    if (auto hresult { CopyFile2(origin.c_str(), destination.c_str(), nullptr) }; FAILED(hresult)) {
        return std::unexpected(hresult);
    }

    return {};
}

auto erase_file(const std::filesystem::path& path) -> std::expected<void, HRESULT> {
    if (auto result { DeleteFileW(path.c_str()) }; result == 0) {
        auto last_error { GetLastError() };
        return std::unexpected(HRESULT_FROM_WIN32(last_error));
    }

    return {};
}

auto download_file(const std::filesystem::path& path, ada::url url)
    -> std::expected<void, HRESULT> {
    auto converted_url { pane::to_utf16(url.get_href()) };

    if (auto hresult { URLDownloadToFileW(nullptr,
                                          reinterpret_cast<const wchar_t*>(converted_url.c_str()),
                                          path.c_str(),
                                          0,
                                          nullptr) };
        hresult != S_OK) {
        return std::unexpected(hresult);
    }

    return {};
}

auto create_symlink(const std::filesystem::path& target, const std::filesystem::path& destination)
    -> std::expected<void, HRESULT> {
    auto flags { std::filesystem::is_directory(target)
                     ? SYMBOLIC_LINK_FLAG_DIRECTORY | SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE
                     : SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE };

    if (auto result {
            CreateSymbolicLinkW(destination.c_str(), target.c_str(), static_cast<::DWORD>(flags)) };
        result == 0) {
        auto last_error { GetLastError() };
        return std::unexpected(HRESULT_FROM_WIN32(last_error));
    }

    return {};
}

auto open_library(const std::filesystem::path& path)
    -> std::expected<wil::com_ptr<IShellLibrary>, HRESULT> {
    wil::com_ptr<IShellLibrary> lib;

    if (auto hresult {
            SHLoadLibraryFromParsingName(path.c_str(), STGM_READWRITE, IID_PPV_ARGS(&lib)) };
        hresult != S_OK) {
        return std::unexpected(hresult);
    }

    return lib;
}

auto open_library(const wil::com_ptr<IShellItem>& item)
    -> std::expected<wil::com_ptr<IShellLibrary>, HRESULT> {
    wil::com_ptr<IShellLibrary> lib;

    if (auto hresult { SHLoadLibraryFromItem(item.get(), STGM_READWRITE, IID_PPV_ARGS(&lib)) };
        hresult != S_OK) {
        return std::unexpected(hresult);
    }

    return lib;
}

auto library_directories(const wil::com_ptr<IShellLibrary>& lib)
    -> std::expected<std::vector<std::filesystem::path>, HRESULT> {
    wil::com_ptr<IShellItemArray> array;

    if (auto hresult { lib->GetFolders(LFF_ALLITEMS, IID_PPV_ARGS(&array)) }; hresult != S_OK) {
        return std::unexpected(hresult);
    }

    DWORD count;
    array->GetCount(&count);

    std::vector<std::filesystem::path> files;
    files.resize(count);

    for (DWORD i = 0; i < count; ++i) {
        wil::com_ptr<IShellItem> item;

        if (auto hresult { array->GetItemAt(i, &item) }; hresult != S_OK) {
            return std::unexpected(hresult);
        }

        wil::unique_cotaskmem_string path;
        if (auto hresult { item->GetDisplayName(SIGDN_FILESYSPATH, &path) }; hresult != S_OK) {
            return std::unexpected(hresult);
        }

        files[i] = path.get();
    }

    return files;
}

auto get_path(const wil::com_ptr<IShellItem>& item) -> std::expected<std::u8string, HRESULT> {
    SFGAOF attributes;

    if (auto hresult { item->GetAttributes(SFGAO_FILESYSTEM, &attributes) }; hresult != S_OK) {
        return std::unexpected(hresult);
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
