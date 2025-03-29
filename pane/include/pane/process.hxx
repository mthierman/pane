#pragma once
#include <Windows.h>
#include <string>
#include <filesystem>
#include <wil/resource.h>

namespace pane {
struct process {
    using Self = process;

    process(const std::filesystem::path& path, std::u8string_view command_line);
    ~process() = default;

    process(const Self& process) = delete;
    auto operator=(const Self& process) -> Self& = delete;
    process(Self&& process) noexcept = delete;
    auto operator=(Self&& process) noexcept -> Self& = delete;

private:
    wil::unique_handle process_handle;
    wil::unique_handle thread_handle;
};
} // namespace pane
