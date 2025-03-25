#include <pane/path.hxx>

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
} // namespace pane
