#include <pane/file_picker.hxx>
#include <pane/system.hxx>

namespace pane {
file_picker::file_picker()
    : dialog { wil::CoCreateInstance<IFileOpenDialog>(CLSID_FileOpenDialog) } {
    dialog->GetOptions(&default_options);
    directory_options = default_options | FOS_PICKFOLDERS & ~FOS_FORCEFILESYSTEM;
    file_options = default_options;
}

auto file_picker::open_directory(this Self& self)
    -> std::expected<wil::com_ptr<IShellItem>, std::error_code> {
    if (auto result { self.dialog->SetOptions(self.directory_options) }; result != S_OK) {
        return std::unexpected(hresult_error(result));
    }

    return self.open();
}

auto file_picker::open_file(this Self& self)
    -> std::expected<wil::com_ptr<IShellItem>, std::error_code> {
    if (auto result { self.dialog->SetOptions(self.file_options) }; result != S_OK) {
        return std::unexpected(hresult_error(result));
    }

    return self.open();
}

auto file_picker::open(this Self& self)
    -> std::expected<wil::com_ptr<IShellItem>, std::error_code> {
    if (auto result { self.dialog->Show(nullptr) }; result != S_OK) {
        return std::unexpected(hresult_error(result));
    }

    wil::com_ptr<IShellItem> item;

    if (auto result { self.dialog->GetResult(&item) }; result != S_OK) {
        return std::unexpected(hresult_error(result));
    }

    return item;
}
} // namespace pane
