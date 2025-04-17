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
    -> std::expected<wil::com_ptr<IShellItem>, HRESULT> {
    if (auto hresult { self.open_dialog->SetOptions(self.options.open.directory) };
        hresult != S_OK) {
        return std::unexpected(hresult);
    }

    return self.open();
}

auto file_picker::open_file(this Self& self) -> std::expected<wil::com_ptr<IShellItem>, HRESULT> {
    if (auto hresult { self.open_dialog->SetOptions(self.options.open.file) }; hresult != S_OK) {
        return std::unexpected(hresult);
    }

    return self.open();
}

auto file_picker::save_file(this Self& self) -> std::expected<wil::com_ptr<IShellItem>, HRESULT> {
    if (auto hresult { self.save_dialog->SetOptions(self.options.save.file) }; hresult != S_OK) {
        return std::unexpected(hresult);
    }

    return self.save();
}

auto file_picker::open(this Self& self) -> std::expected<wil::com_ptr<IShellItem>, HRESULT> {
    if (auto hresult { self.open_dialog->Show(nullptr) }; hresult != S_OK) {
        return std::unexpected(hresult);
    }

    wil::com_ptr<IShellItem> item;

    if (auto hresult { self.open_dialog->GetResult(&item) }; hresult != S_OK) {
        return std::unexpected(hresult);
    }

    return item;
}

auto file_picker::save(this Self& self) -> std::expected<wil::com_ptr<IShellItem>, HRESULT> {
    if (auto hresult { self.save_dialog->Show(nullptr) }; hresult != S_OK) {
        return std::unexpected(hresult);
    }

    wil::com_ptr<IShellItem> item;

    if (auto hresult { self.save_dialog->GetResult(&item) }; hresult != S_OK) {
        return std::unexpected(hresult);
    }

    return item;
}
} // namespace pane
