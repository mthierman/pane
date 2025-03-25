#include <pane/path.hxx>
#include <pane/system.hxx>
#include <wil/resource.h>

// TEMPORARY LOGGING INCLUDE:
#include <comdef.h>
#include <print>

namespace pane {
path::path(std::filesystem::path&& path) noexcept
    : storage { std::move(path) } { }

auto path::operator=(std::filesystem::path&& path) noexcept -> Self& {
    storage = std::move(path);

    return *this;
}

path::path(const string& string) noexcept
    : storage { string.storage } { }

auto path::operator=(const string& string) noexcept -> Self& {
    storage = string.storage;

    return *this;
}

path::path(const hstring& string) noexcept
    : storage { string.storage } { }

auto path::operator=(const hstring& string) noexcept -> Self& {
    storage = string.storage;

    return *this;
}

auto path::from_known_folder(KNOWNFOLDERID known_folder) -> std::expected<Self, std::error_code> {
    wil::unique_cotaskmem_string buffer;

    auto result { ::SHGetKnownFolderPath(known_folder, 0, nullptr, &buffer) };

    if (SUCCEEDED(result)) {
        return Self(buffer.get());
    } else {
        return std::unexpected(hresult_error(result));
    }
}
} // namespace pane
