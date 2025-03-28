#include <pane/file_picker.hxx>
#include <pane/system.hxx>

namespace pane {
file_picker::file_picker()
    : open_dialog { wil::CoCreateInstance<IFileOpenDialog>(CLSID_FileOpenDialog) },
      save_dialog { wil::CoCreateInstance<IFileSaveDialog>(CLSID_FileSaveDialog) } {
    open_dialog->GetOptions(&options.open.defaults);
    save_dialog->GetOptions(&options.save.defaults);

    options.open.directory = options.open.defaults | FOS_PICKFOLDERS & ~FOS_FORCEFILESYSTEM;
    options.open.file = options.open.defaults;
    options.save.file = options.save.defaults;
}

auto file_picker::open_directory(this Self& self)
    -> std::expected<wil::com_ptr<IShellItem>, std::error_code> {
    if (auto result { self.open_dialog->SetOptions(self.options.open.directory) }; result != S_OK) {
        return std::unexpected(hresult_error(result));
    }

    return self.open();
}

auto file_picker::open_file(this Self& self)
    -> std::expected<wil::com_ptr<IShellItem>, std::error_code> {
    if (auto result { self.open_dialog->SetOptions(self.options.open.file) }; result != S_OK) {
        return std::unexpected(hresult_error(result));
    }

    return self.open();
}

auto file_picker::save_file(this Self& self)
    -> std::expected<wil::com_ptr<IShellItem>, std::error_code> {
    if (auto result { self.save_dialog->SetOptions(self.options.save.file) }; result != S_OK) {
        return std::unexpected(hresult_error(result));
    }

    return self.save();
}

auto file_picker::open(this Self& self)
    -> std::expected<wil::com_ptr<IShellItem>, std::error_code> {
    if (auto result { self.open_dialog->Show(nullptr) }; result != S_OK) {
        return std::unexpected(hresult_error(result));
    }

    wil::com_ptr<IShellItem> item;

    if (auto result { self.open_dialog->GetResult(&item) }; result != S_OK) {
        return std::unexpected(hresult_error(result));
    }

    return item;
}

auto file_picker::save(this Self& self)
    -> std::expected<wil::com_ptr<IShellItem>, std::error_code> {
    if (auto result { self.save_dialog->Show(nullptr) }; result != S_OK) {
        return std::unexpected(hresult_error(result));
    }

    wil::com_ptr<IShellItem> item;

    if (auto result { self.open_dialog->GetResult(&item) }; result != S_OK) {
        return std::unexpected(hresult_error(result));
    }

    return item;
}
} // namespace pane
