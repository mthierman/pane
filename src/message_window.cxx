#include <pane/message_window.hxx>

namespace pane {
message_window::message_window(procedure_fn&& window_procedure)
    : window_procedure { std::move(window_procedure) } {
    auto& self = *this;

    message_window_class<Self> window_class { u8"pane_message_window" };

    CreateWindowExW(0,
                    window_class.data.lpszClassName,
                    nullptr,
                    0,
                    0,
                    0,
                    0,
                    0,
                    HWND_MESSAGE,
                    nullptr,
                    window_class.data.hInstance,
                    &self);
}

auto message_window::default_procedure(this Self& self, const window_message& window_message)
    -> LRESULT {
    return window_message.default_procedure();
}
} // namespace pane
