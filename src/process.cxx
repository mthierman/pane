#include <pane/process.hxx>
#include <pane/text.hxx>
#include <wil/resource.h>

namespace pane {
process::process(const std::filesystem::path& path, std::u8string_view command_line) {
    STARTUPINFOW si {};
    si.cb = sizeof(STARTUPINFOW);

    PROCESS_INFORMATION pi {};
    pi.hProcess = process_handle.get();
    pi.hThread = thread_handle.get();

    CreateProcessW(path.c_str(),
                   reinterpret_cast<wchar_t*>(pane::to_utf16(command_line).data()),
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
} // namespace pane
