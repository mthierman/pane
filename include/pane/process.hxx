#pragma once
#include <Windows.h>
#include <string>
#include <filesystem>
#include <wil/resource.h>

namespace pane {
struct process final {
    using Self = process;

    process(const std::filesystem::path& path, std::u8string_view command_line);
    ~process() = default;

    process(const Self&) = delete;
    auto operator=(const Self&) -> Self& = delete;

    process(Self&&) noexcept = delete;
    auto operator=(Self&&) noexcept -> Self& = delete;

private:
    wil::unique_handle process_handle;
    wil::unique_handle thread_handle;
};
} // namespace pane
