#include <pane/window.hxx>

namespace pane {
window::window() {
    if (GetClassInfoExW(
            this->window_class.hInstance, this->window_class.lpszClassName, &this->window_class)
        == 0) {
        RegisterClassExW(&this->window_class);
    }

    this->create();
}

window::window(std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)>&& message_handler) {
    this->message_handler = std::move(message_handler);

    if (GetClassInfoExW(
            this->window_class.hInstance, this->window_class.lpszClassName, &this->window_class)
        == 0) {
        RegisterClassExW(&this->window_class);
    }

    this->create();
}

auto window::window_procedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> LRESULT {
    if (msg == WM_NCCREATE) {
        auto create { reinterpret_cast<CREATESTRUCTW*>(lparam) };

        if (auto self { static_cast<Self*>(create->lpCreateParams) }; self) {
            SetWindowLongPtrW(hwnd, 0, reinterpret_cast<LONG_PTR>(self));
            self->window_handle = hwnd;
        }
    } else {
        if (auto self { reinterpret_cast<window*>(::GetWindowLongPtrW(hwnd, 0)) }; self) {
            if (self->message_handler) {
                return self->message_handler(hwnd, msg, wparam, lparam);
            }
        }
    }

    return DefWindowProcW(hwnd, msg, wparam, lparam);
}

auto window::create() -> void {
    ::CreateWindowExW(0,
                      this->window_class.lpszClassName,
                      this->window_class.lpszClassName,
                      WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_VISIBLE,
                      CW_USEDEFAULT,
                      CW_USEDEFAULT,
                      CW_USEDEFAULT,
                      CW_USEDEFAULT,
                      nullptr,
                      nullptr,
                      this->window_class.hInstance,
                      this);
}
} // namespace pane
