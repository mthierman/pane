#include <pane/process.hpp>
#include <pane/text.hpp>
#include <wil/resource.h>

namespace pane {
process::process(const std::filesystem::path& path, std::u8string_view command_line) {
    STARTUPINFOW si {};
    si.cb = sizeof(STARTUPINFOW);

    PROCESS_INFORMATION pi {};
    pi.hProcess = process_handle.get();
    pi.hThread = thread_handle.get();

    auto cmd = pane::reinterpret_string(pane::utf8_to_utf16_replace(command_line));

    CreateProcessW(
        path.c_str(), cmd.data(), nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi);
    WaitForSingleObject(pi.hProcess, INFINITE);
}
} // namespace pane
