#include "window.hxx"

namespace pane {
auto module_handle() -> std::expected<HMODULE, HRESULT> {
    HMODULE hmodule;

    if (auto result { GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT
                                             | GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
                                         reinterpret_cast<LPCWSTR>(&module_handle),
                                         &hmodule) };
        result == 0) {
        auto last_error { GetLastError() };
        return std::unexpected(HRESULT_FROM_WIN32(last_error));
    };

    return hmodule;
}

auto message::default_procedure() const -> LRESULT {
    return DefWindowProcW(this->hwnd, this->msg, this->wparam, this->lparam);
}

auto message::send(HWND hwnd) const -> LRESULT {
    return SendMessageW(hwnd ? hwnd : this->hwnd, this->msg, this->wparam, this->lparam);
}

auto message::post(HWND hwnd) const -> BOOL {
    return PostMessageW(hwnd ? hwnd : this->hwnd, this->msg, this->wparam, this->lparam);
}

message_window::message_window() {
    CreateWindowExW(0,
                    this->window_class.data().lpszClassName,
                    nullptr,
                    0,
                    0,
                    0,
                    0,
                    0,
                    HWND_MESSAGE,
                    nullptr,
                    this->window_class.data().hInstance,
                    this);
}

auto message_window::default_procedure(const message& message) -> LRESULT {
    return message.default_procedure();
}
} // namespace pane
