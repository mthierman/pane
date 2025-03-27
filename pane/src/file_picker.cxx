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

    // SFGAOF attributes;

    // if (auto result { item->GetAttributes(SFGAO_FILESYSTEM, &attributes) }; result != S_OK) {
    //     return std::unexpected(hresult_error(result));
    // }

    // wil::unique_cotaskmem_string buffer;
    // // item->GetDisplayName(SIGDN_FILESYSPATH, &buffer);
    // // item->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &buffer);

    // if (attributes) {
    //     // OutputDebugStringW(L"SFGAO_FILESYSTEM IS TRUE");
    //     item->GetDisplayName(SIGDN_FILESYSPATH, &buffer);
    //     return file();
    // } else {
    //     // OutputDebugStringW(L"SFGAO_FILESYSTEM IS FALSE");
    //     item->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &buffer);
    // }

    // OutputDebugStringW(buffer.get());

    // // auto display_name { pane::string::from_utf16(buffer.get()) };

    // // MessageBoxA(nullptr,
    // //             std::format("Result: {}\n Item: {}",
    // //                         hresult_error(result),
    // //                         display_name.value_or(pane::string()).c_str())
    // //                 .c_str(),
    // //             "",
    // //             MB_OK);
}
} // namespace pane
