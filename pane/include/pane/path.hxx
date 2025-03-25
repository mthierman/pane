#pragma once
#include <string>
#include <filesystem>

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

    std::filesystem::path storage;
};
} // namespace pane
