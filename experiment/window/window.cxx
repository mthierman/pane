#include "window.hxx"

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

auto window_message::default_procedure(this const Self& self) -> LRESULT {
    return DefWindowProcW(self.hwnd, self.msg, self.wparam, self.lparam);
}

auto window_message::send(this const Self& self, HWND hwnd) -> LRESULT {
    return SendMessageW(hwnd ? hwnd : self.hwnd, self.msg, self.wparam, self.lparam);
}

auto window_message::post(this const Self& self, HWND hwnd) -> BOOL {
    return PostMessageW(hwnd ? hwnd : self.hwnd, self.msg, self.wparam, self.lparam);
}

message_window::message_window() {
    auto& self = *this;

    message_window_class<Self> window_class;

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
