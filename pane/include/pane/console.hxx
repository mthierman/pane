#pragma once
#include <Windows.h>
#include <wil/resource.h>

namespace pane {
struct console {
    using Self = console;

    console();
    ~console();

    console(Self&& console) noexcept = delete;
    auto operator=(Self&& console) noexcept -> Self& = delete;

    console(const Self& console) = delete;
    auto operator=(const Self& console) -> Self& = delete;

private:
    wil::unique_file file;
};
} // namespace pane
