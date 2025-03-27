#include <pane/file_picker.hxx>
#include <pane/system.hxx>

// LOGGING
#include <format>
#include <print>
#include <pane/string.hxx>

namespace pane {
// https://learn.microsoft.com/en-us/windows/win32/shell/common-file-dialog
// https://learn.microsoft.com/en-us/windows/win32/shell/library-be-library-aware
auto file_picker::open(this Self& /* self */) -> void {
    //
    auto open_dialog { wil::CoCreateInstance<IFileOpenDialog>(CLSID_FileOpenDialog) };

    // FILEOPENDIALOGOPTIONS options;
    // open_dialog->GetOptions(&options);
    // open_dialog->SetOptions(options | FOS_PICKFOLDERS | ~FOS_FORCEFILESYSTEM);

    FILEOPENDIALOGOPTIONS options;
    open_dialog->GetOptions(&options);
    open_dialog->SetOptions(options | FOS_PICKFOLDERS & ~FOS_FORCEFILESYSTEM);

    // auto result { open_dialog->Show(nullptr) };
    open_dialog->Show(nullptr);

    wil::com_ptr<IShellItem> item;
    open_dialog->GetResult(&item);
    wil::unique_cotaskmem_string buffer;
    // item->GetDisplayName(SIGDN_FILESYSPATH, &buffer);
    item->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &buffer);
    OutputDebugStringW(buffer.get());

    // auto display_name { pane::string::from_utf16(buffer.get()) };

    // MessageBoxA(nullptr,
    //             std::format("Result: {}\n Item: {}",
    //                         hresult_error(result),
    //                         display_name.value_or(pane::string()).c_str())
    //                 .c_str(),
    //             "",
    //             MB_OK);
}
} // namespace pane
