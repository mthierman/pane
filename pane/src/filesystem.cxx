#include <pane/filesystem.hxx>
#include <pane/system.hxx>
#include <pane/string.hxx>
#include <pane/hstring.hxx>
#include <urlmon.h>

// LOGGING
#include <print>

namespace pane {
file::file(std::filesystem::path&& path) noexcept
    : storage { std::move(path) } { }

auto file::operator=(std::filesystem::path&& path) noexcept -> Self& {
    storage = std::move(path);

    return *this;
}

file::file(const string& string) noexcept
    : storage { string.storage } { }

auto file::operator=(const string& string) noexcept -> Self& {
    storage = string.storage;

    return *this;
}

file::file(const hstring& string) noexcept
    : storage { string.storage } { }

auto file::operator=(const hstring& string) noexcept -> Self& {
    storage = string.storage;

    return *this;
}

auto file::from_known_folder(KNOWNFOLDERID known_folder) -> std::expected<Self, std::error_code> {
    wil::unique_cotaskmem_string buffer;

    auto result { ::SHGetKnownFolderPath(known_folder, KF_FLAG_DONT_VERIFY, nullptr, &buffer) };

    if (FAILED(result)) {
        return std::unexpected(hresult_error(result));
    }

    return Self(buffer.get());
}

auto file::from_temp_folder() -> std::expected<Self, std::error_code> {
    std::wstring buffer;

    auto length { ::GetTempPath2W(0, buffer.data()) };

    if (length == 0) {
        return std::unexpected(last_error());
    }

    buffer.resize(length);

    if (::GetTempPath2W(length, buffer.data()) == 0) {
        return std::unexpected(last_error());
    }

    buffer.resize(buffer.size() - 2);

    return Self(buffer);
}

auto file::create_directory() -> std::expected<void, std::error_code> {
    if (CreateDirectoryW(storage.c_str(), nullptr) == 0) {
        return std::unexpected(last_error());
    }

    return {};
}

auto file::create_directory_from_template(const Self& template_directory)
    -> std::expected<void, std::error_code> {
    if (::CreateDirectoryExW(template_directory.storage.c_str(), storage.c_str(), nullptr) == 0) {
        return std::unexpected(last_error());
    }

    return {};
}

auto file::create() -> bool {
    if (::CreateFile2(storage.c_str(), 0, 0, CREATE_NEW, nullptr) == INVALID_HANDLE_VALUE) {
        return false;
    }

    return true;
}

auto file::open() -> std::expected<wil::unique_handle, std::error_code> {
    auto handle { wil::unique_handle(
        ::CreateFile2(storage.c_str(), GENERIC_READ | GENERIC_WRITE, 0, OPEN_EXISTING, nullptr)) };

    if (handle.get() == INVALID_HANDLE_VALUE) {
        return std::unexpected(last_error());
    }

    return handle;
}

auto file::move(const Self& destination) -> std::expected<void, std::error_code> {
    if (::MoveFileW(storage.c_str(), destination.storage.c_str()) == 0) {
        return std::unexpected(last_error());
    }

    return {};
}

auto file::copy(const Self& destination) -> std::expected<void, std::error_code> {
    auto result { ::CopyFile2(storage.c_str(), destination.storage.c_str(), nullptr) };

    if (FAILED(result)) {
        return std::unexpected(hresult_error(result));
    }

    return {};
}

auto file::erase() -> std::expected<void, std::error_code> {
    if (::DeleteFileW(storage.c_str()) == 0) {
        return std::unexpected(last_error());
    }

    return {};
}

auto file::create_symlink(const Self& destination) -> std::expected<void, std::error_code> {
    auto flags { std::filesystem::is_directory(storage)
                     ? SYMBOLIC_LINK_FLAG_DIRECTORY | SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE
                     : SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE };

    if (::CreateSymbolicLinkW(
            destination.storage.c_str(), storage.c_str(), static_cast<::DWORD>(flags))
        == 0) {
        return std::unexpected(last_error());
    }

    return {};
}

auto file::open_library() -> std::expected<wil::com_ptr<IShellLibrary>, std::error_code> {
    auto co_initialize { wil::CoInitializeEx() };

    wil::com_ptr<IShellLibrary> lib;

    if (auto result { SHLoadLibraryFromParsingName(
            L"C:\\Users\\mthie\\AppData\\Roaming\\Microsoft\\Windows\\Libraries\\Samples.library-"
            L"ms",
            STGM_READWRITE,
            IID_PPV_ARGS(&lib)) };
        result != S_OK) {
        return std::unexpected(hresult_error(result));
    }

    return lib;
}

auto file::library_directories(const wil::com_ptr<IShellLibrary>& lib)
    -> std::expected<std::vector<Self>, std::error_code> {
    auto co_initialize { wil::CoInitializeEx() };

    wil::com_ptr<IShellItemArray> array;

    if (auto result { lib->GetFolders(LFF_ALLITEMS, IID_PPV_ARGS(&array)) }; FAILED(result)) {
        return std::unexpected(hresult_error(result));
    }

    DWORD count;
    array->GetCount(&count);

    std::vector<Self> files;
    files.resize(count);

    for (DWORD i = 0; i < count; ++i) {
        wil::com_ptr<IShellItem> item;

        if (auto result { array->GetItemAt(i, &item) }; result != S_OK) {
            return std::unexpected(hresult_error(result));
        }

        wil::unique_cotaskmem_string path;
        if (auto result { item->GetDisplayName(SIGDN_FILESYSPATH, &path) }; result != S_OK) {
            return std::unexpected(hresult_error(result));
        }

        files[i] = path.get();
    }

    return files;
}

// auto download(std::u8string_view url, const std::filesystem::path& path)
//     -> std::expected<Self, std::u8string> {
//     if (auto u16url { pane::hstring::from_utf8(url) }) {
//         // We need to check if the directories exist on path
//         if (auto result {
//                 ::URLDownloadToFileW(nullptr, u16url.value().c_str(), path.c_str(), 0, nullptr)
//                 };
//             result == S_OK) {
//             return open_existing(path);
//         } else {
//             return std::unexpected(pane::last_error());
//         }
//     } else {
//         return std::unexpected(u16url.error());
//     }
// }
} // namespace pane
