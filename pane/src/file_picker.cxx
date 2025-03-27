#include <pane/file_picker.hxx>
#include <pane/system.hxx>

namespace pane {
auto file_picker::open_directory(this Self& /* self */)
    -> std::expected<wil::com_ptr<IShellItem>, std::error_code> {
    auto dialog { wil::CoCreateInstance<IFileOpenDialog>(CLSID_FileOpenDialog) };

    FILEOPENDIALOGOPTIONS options;

    if (auto result { dialog->GetOptions(&options) }; result != S_OK) {
        return std::unexpected(hresult_error(result));
    }

    if (auto result { dialog->SetOptions(options | FOS_PICKFOLDERS & ~FOS_FORCEFILESYSTEM) };
        result != S_OK) {
        return std::unexpected(hresult_error(result));
    }

    if (auto result { dialog->Show(nullptr) }; result != S_OK) {
        return std::unexpected(hresult_error(result));
    }

    wil::com_ptr<IShellItem> item;

    if (auto result { dialog->GetResult(&item) }; result != S_OK) {
        return std::unexpected(hresult_error(result));
    }

    return item;
}

auto file_picker::open_file(this Self& /* self */)
    -> std::expected<wil::com_ptr<IShellItem>, std::error_code> {
    auto dialog { wil::CoCreateInstance<IFileOpenDialog>(CLSID_FileOpenDialog) };

    FILEOPENDIALOGOPTIONS options;

    if (auto result { dialog->GetOptions(&options) }; result != S_OK) {
        return std::unexpected(hresult_error(result));
    }

    if (auto result { dialog->SetOptions(options) }; result != S_OK) {
        return std::unexpected(hresult_error(result));
    }

    if (auto result { dialog->Show(nullptr) }; result != S_OK) {
        return std::unexpected(hresult_error(result));
    }

    wil::com_ptr<IShellItem> item;

    if (auto result { dialog->GetResult(&item) }; result != S_OK) {
        return std::unexpected(hresult_error(result));
    }

    return item;
}

// auto file_picker::open(this Self& /* self */, FILEOPENDIALOGOPTIONS options)
//     -> std::expected<wil::com_ptr<IShellItem>, std::error_code> {
//     auto dialog { wil::CoCreateInstance<IFileOpenDialog>(CLSID_FileOpenDialog) };

//     if (auto result { dialog->GetOptions(&options) }; result != S_OK) {
//         return std::unexpected(hresult_error(result));
//     }

//     if (auto result { dialog->SetOptions(options | FOS_PICKFOLDERS & ~FOS_FORCEFILESYSTEM) };
//         result != S_OK) {
//         return std::unexpected(hresult_error(result));
//     }

//     if (auto result { dialog->Show(nullptr) }; result != S_OK) {
//         return std::unexpected(hresult_error(result));
//     }

//     wil::com_ptr<IShellItem> item;

//     if (auto result { dialog->GetResult(&item) }; result != S_OK) {
//         return std::unexpected(hresult_error(result));
//     }

//     return item;
// }
} // namespace pane
