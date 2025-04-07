#include <pane/window.hxx>
#include <pane/debug.hxx>

namespace pane {
window::window(pane::window::config&& window_config,
               std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)>&& window_procedure)
    : window_config { std::move(window_config) },
      window_procedure { std::move(window_procedure) } {
    if (GetClassInfoExW(
            this->window_class.hInstance, this->window_class.lpszClassName, &this->window_class)
        == 0) {
        RegisterClassExW(&this->window_class);
    };

    CreateWindowExW(0,
                    this->window_class.lpszClassName,
                    this->window_class.lpszClassName,
                    WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
                    CW_USEDEFAULT,
                    CW_USEDEFAULT,
                    CW_USEDEFAULT,
                    CW_USEDEFAULT,
                    nullptr,
                    nullptr,
                    this->window_class.hInstance,
                    this);

    if (this->window_config.visible) {
        this->activate();
    }
}

auto window::activate(this const Self& self) -> bool {
    return ShowWindow(self.hwnd(), SW_SHOWNORMAL);
}

auto window::class_window_procedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> LRESULT {
    if (msg == WM_NCCREATE) {
        if (auto create { reinterpret_cast<CREATESTRUCTW*>(lparam) }) {
            if (auto self { static_cast<Self*>(create->lpCreateParams) }) {
                SetWindowLongPtrW(hwnd, 0, reinterpret_cast<LONG_PTR>(self));
                self->window_handle.reset(hwnd);
            }
        }
    }

    if (auto self { reinterpret_cast<Self*>(GetWindowLongPtrW(hwnd, 0)) }) {
        if (self->window_procedure) {
            return self->window_procedure(hwnd, msg, wparam, lparam);
        }
    }

    return DefWindowProcW(hwnd, msg, wparam, lparam);
}

auto window::hwnd(this const Self& self) -> HWND { return self.window_handle.get(); }
} // namespace pane
