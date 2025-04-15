#pragma once
#include <Windows.h>
#include <wil/resource.h>

namespace pane {
struct console final {
    using Self = console;

    console();
    ~console();

    console(const Self&) = delete;
    auto operator=(const Self&) -> Self& = delete;

    console(Self&&) noexcept = delete;
    auto operator=(Self&&) noexcept -> Self& = delete;

private:
    wil::unique_file file;
};
} // namespace pane
