#pragma once
#include <Windows.h>
#include <ShlObj.h>
#include <expected>
#include <system_error>
#include <wil/com.h>
// #include <pane/file.hxx>
#include <pane/co_init.hxx>

// https://learn.microsoft.com/en-us/windows/win32/shell/common-file-dialog
// https://learn.microsoft.com/en-us/windows/win32/shell/library-be-library-aware
// https://learn.microsoft.com/en-us/windows/win32/shell/sfgao
// https://stackoverflow.com/questions/16543771/what-is-the-correct-way-to-detect-that-an-ishellitem-refers-to-a-library

namespace pane {
struct file_picker {
    using Self = file_picker;

    file_picker() = default;
    ~file_picker() = default;

    file_picker(Self&& file) noexcept = default;
    auto operator=(Self&& file) noexcept -> Self& = default;

    file_picker(const Self& file) = default;
    auto operator=(const Self& file) -> Self& = default;

    auto open_directory(this Self& self)
        -> std::expected<wil::com_ptr<IShellItem>, std::error_code>;
    auto open_file(this Self& self) -> std::expected<wil::com_ptr<IShellItem>, std::error_code>;

private:
    auto open(this Self& self, FILEOPENDIALOGOPTIONS options)
        -> std::expected<wil::com_ptr<IShellItem>, std::error_code>;
};
} // namespace pane
