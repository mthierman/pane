#pragma once
#include <Windows.h>

namespace pane {
struct process {
    using Self = process;

    process();
    ~process();

    process(Self&& process) noexcept = delete;
    auto operator=(Self&& process) noexcept -> Self& = delete;

    process(const Self& process) = delete;
    auto operator=(const Self& process) -> Self& = delete;

private:
};
} // namespace pane
