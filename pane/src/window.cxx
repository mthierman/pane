#include <pane/window.hxx>

namespace pane {
window::window() {
    this->register_class();
    this->create();
}

window::window(std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)>&& message_handler) {
    this->message_handler = std::move(message_handler);
    this->register_class();
    this->create();
}

auto window::window_procedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> LRESULT {
    if (msg == WM_NCCREATE) {
        if (auto create { reinterpret_cast<CREATESTRUCTW*>(lparam) }) {
            if (auto self { static_cast<Self*>(create->lpCreateParams) }) {
                SetWindowLongPtrW(hwnd, 0, reinterpret_cast<LONG_PTR>(self));
                self->window_handle.reset(hwnd);
            }
        }
    }

    if (auto self { reinterpret_cast<Self*>(GetWindowLongPtrW(hwnd, 0)) }) {
        return self->message_handler(hwnd, msg, wparam, lparam);
    }

    return DefWindowProcW(hwnd, msg, wparam, lparam);
}

auto window::register_class(this Self& self) -> void {
    if (GetClassInfoExW(
            self.window_class.hInstance, self.window_class.lpszClassName, &self.window_class)
        == 0) {
        RegisterClassExW(&self.window_class);
    };
}

auto window::create(this Self& self) -> HWND {
    return CreateWindowExW(0,
                           self.window_class.lpszClassName,
                           self.window_class.lpszClassName,
                           WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
                           CW_USEDEFAULT,
                           CW_USEDEFAULT,
                           CW_USEDEFAULT,
                           CW_USEDEFAULT,
                           nullptr,
                           nullptr,
                           self.window_class.hInstance,
                           &self);
}

auto window::activate(this Self& self) -> bool {
    return ShowWindow(self.window_handle, SW_SHOWNORMAL);
}
} // namespace pane
