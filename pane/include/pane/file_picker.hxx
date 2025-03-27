#pragma once
#include <Windows.h>
#include <ShlObj.h>
#include <expected>
#include <wil/com.h>
#include <pane/file.hxx>

// https://learn.microsoft.com/en-us/windows/win32/shell/common-file-dialog
// https://learn.microsoft.com/en-us/windows/win32/shell/library-be-library-aware
// https://learn.microsoft.com/en-us/windows/win32/shell/sfgao

namespace pane {
struct file_picker {
    using Self = file_picker;

    file_picker() = default;
    ~file_picker() = default;

    file_picker(Self&& file) noexcept = default;
    auto operator=(Self&& file) noexcept -> Self& = default;

    file_picker(const Self& file) = default;
    auto operator=(const Self& file) -> Self& = default;

    auto open_directory(this Self& self) -> std::expected<file, std::error_code>;
};
} // namespace pane
