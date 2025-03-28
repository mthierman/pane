#include <pane/process.hxx>
#include <pane/hstring.hxx>
#include <wil/resource.h>

namespace pane {
process::process(const file& file, const string& command_line) {
    STARTUPINFOW si {};
    si.cb = sizeof(STARTUPINFOW);

    PROCESS_INFORMATION pi {};
    pi.hProcess = process_handle.get();
    pi.hThread = thread_handle.get();

    if (auto converted_command_line { hstring::from_utf8(command_line.get()) }) {
        CreateProcessW(file.storage.c_str(),
                       converted_command_line.value().c_str(),
                       nullptr,
                       nullptr,
                       FALSE,
                       0,
                       nullptr,
                       nullptr,
                       &si,
                       &pi);
        WaitForSingleObject(pi.hProcess, INFINITE);
    }
}
} // namespace pane
