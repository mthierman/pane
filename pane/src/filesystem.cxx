#include <pane/filesystem.hxx>
#include <pane/system.hxx>
#include <pane/hstring.hxx>
#include <urlmon.h>
#include <print>

namespace pane::fs {
auto file::create_always(const std::filesystem::path& path) -> std::expected<Self, std::u8string> {
    if (auto handle {
            ::CreateFile2(path.c_str(), GENERIC_READ | GENERIC_WRITE, 0, CREATE_ALWAYS, nullptr) };
        handle != INVALID_HANDLE_VALUE) {
        return Self { .handle { handle } };
    } else {
        return std::unexpected(pane::sys::last_error());
    }
}

auto file::create_new(const std::filesystem::path& path) -> std::expected<Self, std::u8string> {
    if (auto handle {
            ::CreateFile2(path.c_str(), GENERIC_READ | GENERIC_WRITE, 0, CREATE_NEW, nullptr) };
        handle != INVALID_HANDLE_VALUE) {
        return Self { .handle { handle } };
    } else {
        return std::unexpected(pane::sys::last_error());
    }
}

auto file::open_always(const std::filesystem::path& path) -> std::expected<Self, std::u8string> {
    if (auto handle {
            ::CreateFile2(path.c_str(), GENERIC_READ | GENERIC_WRITE, 0, OPEN_ALWAYS, nullptr) };
        handle != INVALID_HANDLE_VALUE) {
        return Self { .handle { handle } };
    } else {
        return std::unexpected(pane::sys::last_error());
    }
}

auto file::open_existing(const std::filesystem::path& path) -> std::expected<Self, std::u8string> {
    if (auto handle {
            ::CreateFile2(path.c_str(), GENERIC_READ | GENERIC_WRITE, 0, OPEN_EXISTING, nullptr) };
        handle != INVALID_HANDLE_VALUE) {
        return Self { .handle { handle } };
    } else {
        return std::unexpected(pane::sys::last_error());
    }
}

auto file::from_url(std::u8string_view url, const std::filesystem::path& path)
    -> std::expected<Self, std::u8string> {
    if (auto u16url { pane::hstring::from_utf8(url) }) {
        // We need to check if the directories exist on path
        if (auto result {
                ::URLDownloadToFileW(nullptr, u16url.value().c_str(), path.c_str(), 0, nullptr) };
            result == S_OK) {
            return open_existing(path);
        } else {
            return std::unexpected(pane::sys::last_error());
        }
    } else {
        return std::unexpected(u16url.error());
    }
}

auto library::create_from_name() -> ::HRESULT {
    IShellLibrary* lib;
    auto result { SHLoadLibraryFromParsingName(
        L"C:\\Users\\mthie\\AppData\\Roaming\\Microsoft\\Windows\\Libraries\\Samples.library-ms",
        STGM_READWRITE,
        IID_PPV_ARGS(&lib)) };

    return result;
}

auto library::create() -> ::IShellLibrary* {
    IShellLibrary* lib;

    // auto result { SHLoadLibraryFromParsingName(
    //     L"C:\\Users\\mthie\\AppData\\Roaming\\Microsoft\\Windows\\Libraries\\Samples.library-ms",
    //     STGM_READWRITE,
    //     IID_PPV_ARGS(&lib)) };

    // std::println("{}", pane::sys::format_message(result));

    SHLoadLibraryFromParsingName(
        L"C:\\Users\\mthie\\AppData\\Roaming\\Microsoft\\Windows\\Libraries\\Samples.library-ms",
        STGM_READWRITE,
        IID_PPV_ARGS(&lib));

    return lib;
}

auto library::get_folders(::IShellLibrary* lib) -> std::vector<std::u8string> {
    IShellItemArray* array;
    // auto folders { lib->GetFolders(LFF_ALLITEMS, IID_PPV_ARGS(&array)) };
    lib->GetFolders(LFF_ALLITEMS, IID_PPV_ARGS(&array));

    DWORD count;
    IShellItem* item;
    array->GetCount(&count);
    array->GetItemAt(0, &item);
    wil::unique_cotaskmem_string name;
    item->GetDisplayName(SIGDN_FILESYSPATH, &name);

    std::println("{}", count);

    OutputDebugStringW(name.get());

    // std::println("{}", name);

    return {};
}

// auto library::create() -> std::expected<Self, std::u8string> {
//     //
//     // auto shell_library { wil::CoCreateInstance<::IShellLibrary>(CLSID_ShellLibrary) };
//     IShellLibrary* lib;
//     auto result { SHLoadLibraryFromParsingName(
//         L"TEST", STGM_READWRITE, IID_PPV_ARGS(&i_shell_library)) };
// }

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

auto create_directory(const std::filesystem::path& path)
    -> std::expected<std::filesystem::path, std::u8string> {
    if (auto result { ::CreateDirectoryW(path.c_str(), nullptr) }; result != 0) {
        return path;
    } else {
        return std::unexpected(pane::sys::last_error());
    }
}

auto create_directory(const std::filesystem::path& path,
                      const std::filesystem::path& template_directory)
    -> std::expected<std::filesystem::path, std::u8string> {
    if (auto result { ::CreateDirectoryExW(template_directory.c_str(), path.c_str(), nullptr) };
        result != 0) {
        return path;
    } else {
        return std::unexpected(pane::sys::last_error());
    }
}

auto move_file(const std::filesystem::path& origin, const std::filesystem::path& destination)
    -> std::expected<std::filesystem::path, std::u8string> {
    if (auto result { ::MoveFileW(origin.c_str(), destination.c_str()) }; result != 0) {
        return destination;
    } else {
        return std::unexpected(pane::sys::last_error());
    }
}

auto copy_file(const std::filesystem::path& origin, const std::filesystem::path& destination)
    -> std::expected<std::filesystem::path, std::u8string> {
    if (auto result { ::CopyFile2(origin.c_str(), destination.c_str(), nullptr) };
        SUCCEEDED(result)) {
        return destination;
    } else {
        return std::unexpected(pane::sys::last_error());
    }
}

auto erase_file(const std::filesystem::path& path)
    -> std::expected<std::filesystem::path, std::u8string> {
    if (auto result { ::DeleteFileW(path.c_str()) }; result != 0) {
        return path;
    } else {
        return std::unexpected(pane::sys::last_error());
    }
}

auto create_symlink(const std::filesystem::path& target, const std::filesystem::path& destination)
    -> std::expected<std::filesystem::path, std::u8string> {
    auto flags { std::filesystem::is_directory(target)
                     ? SYMBOLIC_LINK_FLAG_DIRECTORY | SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE
                     : SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE };

    if (auto result { ::CreateSymbolicLinkW(
            destination.c_str(), target.c_str(), static_cast<::DWORD>(flags)) };
        result != 0) {
        return destination;
    } else {
        return std::unexpected(pane::sys::last_error());
    }
}
} // namespace pane::fs
