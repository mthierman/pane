#include <pane/message_box.hxx>
#include <pane/hstring.hxx>

namespace pane {
auto message_box(string message, string title) -> void {
    auto converted_message { hstring::from_utf8(message.storage) };
    auto converted_title { hstring::from_utf8(title.storage) };

    if (converted_message && converted_title) {
        MessageBoxW(nullptr,
                    converted_message.value().c_str(),
                    converted_title.value().c_str(),
                    MB_OK | MB_ICONASTERISK);
    }
}

// auto error_box(std::u8string message, std::u8string title) -> void {
//     MessageBoxW(nullptr,
//                 std::vformat(format_string.get(), std::make_wformat_args(args...)).c_str(),
//                 L"Error",
//                 MB_OK | MB_ICONHAND);
// }
} // namespace pane
