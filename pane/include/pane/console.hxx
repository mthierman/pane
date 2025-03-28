#pragma once
#include <Windows.h>

namespace pane {
struct console {
    using Self = console;

    console();
    ~console();

    console(Self&& console) noexcept = default;
    auto operator=(Self&& console) noexcept -> Self& = default;

    console(const Self& console) = delete;
    auto operator=(const Self& console) -> Self& = delete;

private:
};
} // namespace pane
