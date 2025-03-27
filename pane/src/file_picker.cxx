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
    auto dialog { wil::CoCreateInstance<IFileOpenDialog>(CLSID_FileOpenDialog) };

    HRESULT result;

    FILEOPENDIALOGOPTIONS options;
    result = dialog->GetOptions(&options);
    result = dialog->SetOptions(options | FOS_PICKFOLDERS & ~FOS_FORCEFILESYSTEM);
    result = dialog->Show(nullptr);

    wil::com_ptr<IShellItem> item;
    result = dialog->GetResult(&item);

    wil::unique_cotaskmem_string buffer;
    // item->GetDisplayName(SIGDN_FILESYSPATH, &buffer);
    // item->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &buffer);

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
