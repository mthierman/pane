#include <pane/path.hxx>

namespace pane {
path::path(std::filesystem::path&& path) noexcept
    : storage { std::move(path) } { }

auto path::operator=(std::filesystem::path&& path) noexcept -> Self& {
    storage = std::move(path);

    return *this;
}
} // namespace pane
