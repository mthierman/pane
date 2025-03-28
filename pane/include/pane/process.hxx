#pragma once
#include <Windows.h>
#include <pane/file.hxx>
#include <pane/string.hxx>

namespace pane {
struct process {
    using Self = process;

    process(const file& file, const string& command_line);
    ~process() = default;

    process(Self&& process) noexcept = delete;
    auto operator=(Self&& process) noexcept -> Self& = delete;

    process(const Self& process) = delete;
    auto operator=(const Self& process) -> Self& = delete;

private:
    wil::unique_handle process_handle;
    wil::unique_handle thread_handle;
};
} // namespace pane
