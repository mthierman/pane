#include <pane/file.hxx>
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

file::file(const hstring& hstring) noexcept
    : storage { hstring.storage } { }

auto file::operator=(const hstring& hstring) noexcept -> Self& {
    storage = hstring.storage;

    return *this;
}

auto file::from_known_folder(KNOWNFOLDERID known_folder) -> std::expected<Self, std::error_code> {
    wil::unique_cotaskmem_string buffer;

    if (auto result { ::SHGetKnownFolderPath(known_folder, KF_FLAG_DONT_VERIFY, nullptr, &buffer) };
        FAILED(result)) {
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

auto file::create_directory(this Self& self) -> std::expected<void, std::error_code> {
    if (CreateDirectoryW(self.storage.c_str(), nullptr) == 0) {
        return std::unexpected(last_error());
    }

    return {};
}

auto file::create_directory_from_template(this Self& self, const Self& template_directory)
    -> std::expected<void, std::error_code> {
    if (::CreateDirectoryExW(template_directory.storage.c_str(), self.storage.c_str(), nullptr)
        == 0) {
        return std::unexpected(last_error());
    }

    return {};
}

auto file::create(this Self& self) -> bool {
    if (::CreateFile2(self.storage.c_str(), 0, 0, CREATE_NEW, nullptr) == INVALID_HANDLE_VALUE) {
        return false;
    }

    return true;
}

auto file::open(this Self& self) -> std::expected<wil::unique_handle, std::error_code> {
    auto handle { wil::unique_handle(::CreateFile2(
        self.storage.c_str(), GENERIC_READ | GENERIC_WRITE, 0, OPEN_EXISTING, nullptr)) };

    if (handle.get() == INVALID_HANDLE_VALUE) {
        return std::unexpected(last_error());
    }

    return handle;
}

auto file::move(this Self& self, const Self& destination) -> std::expected<void, std::error_code> {
    if (::MoveFileW(self.storage.c_str(), destination.storage.c_str()) == 0) {
        return std::unexpected(last_error());
    }

    return {};
}

auto file::copy(this Self& self, const Self& destination) -> std::expected<void, std::error_code> {
    if (auto result { ::CopyFile2(self.storage.c_str(), destination.storage.c_str(), nullptr) };
        FAILED(result)) {
        return std::unexpected(hresult_error(result));
    }

    return {};
}

auto file::erase(this Self& self) -> std::expected<void, std::error_code> {
    if (::DeleteFileW(self.storage.c_str()) == 0) {
        return std::unexpected(last_error());
    }

    return {};
}

auto file::create_symlink(this Self& self, const Self& destination)
    -> std::expected<void, std::error_code> {
    auto flags { std::filesystem::is_directory(self.storage)
                     ? SYMBOLIC_LINK_FLAG_DIRECTORY | SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE
                     : SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE };

    if (::CreateSymbolicLinkW(
            destination.storage.c_str(), self.storage.c_str(), static_cast<::DWORD>(flags))
        == 0) {
        return std::unexpected(last_error());
    }

    return {};
}

auto file::open_library(this Self& self)
    -> std::expected<wil::com_ptr<IShellLibrary>, std::error_code> {
    auto co_initialize { wil::CoInitializeEx() };

    wil::com_ptr<IShellLibrary> lib;

    if (auto result { SHLoadLibraryFromParsingName(
            self.storage.c_str(), STGM_READWRITE, IID_PPV_ARGS(&lib)) };
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

auto file::download_from_url(this Self& self, url url) -> std::expected<void, std::error_code> {
    auto converted_url { pane::hstring::from_utf8(url.storage.get_href()) };

    if (!converted_url) {
        return std::unexpected(converted_url.error());
    }

    if (auto result { ::URLDownloadToFileW(
            nullptr, converted_url.value().c_str(), self.storage.c_str(), 0, nullptr) };
        FAILED(result)) {
        return std::unexpected(hresult_error(result));
    }

    return {};
}
} // namespace pane
