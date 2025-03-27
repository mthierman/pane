#pragma once
#include <Windows.h>
#include <ShlObj.h>
#include <wil/com.h>

namespace pane {
struct file_picker {
    using Self = file_picker;

    file_picker() = default;
    ~file_picker() = default;

    file_picker(Self&& file) noexcept = default;
    auto operator=(Self&& file) noexcept -> Self& = default;

    file_picker(const Self& file) = default;
    auto operator=(const Self& file) -> Self& = default;

    auto open(this Self& self) -> void;
};
} // namespace pane
