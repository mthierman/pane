#pragma once
#include <Windows.h>
#include <ShlObj.h>
#include <expected>
#include <system_error>
#include <wil/com.h>

namespace pane {
struct file_picker final {
    using Self = file_picker;

    file_picker();
    ~file_picker() = default;

    file_picker(const Self&) = default;
    auto operator=(const Self&) -> Self& = default;

    file_picker(Self&&) noexcept = default;
    auto operator=(Self&&) noexcept -> Self& = default;

    auto open_directory(this Self& self) -> std::expected<wil::com_ptr<IShellItem>, HRESULT>;
    auto open_file(this Self& self) -> std::expected<wil::com_ptr<IShellItem>, HRESULT>;
    auto save_file(this Self& self) -> std::expected<wil::com_ptr<IShellItem>, HRESULT>;

private:
    auto open(this Self& self) -> std::expected<wil::com_ptr<IShellItem>, HRESULT>;
    auto save(this Self& self) -> std::expected<wil::com_ptr<IShellItem>, HRESULT>;

    struct options {
        struct open {
            FILEOPENDIALOGOPTIONS defaults;
            FILEOPENDIALOGOPTIONS directory;
            FILEOPENDIALOGOPTIONS file;
        };

        struct save {
            FILEOPENDIALOGOPTIONS defaults;
            FILEOPENDIALOGOPTIONS file;
        };

        open open;
        save save;
    };

    wil::com_ptr<IFileOpenDialog> open_dialog;
    wil::com_ptr<IFileSaveDialog> save_dialog;
    options options;
};
} // namespace pane
