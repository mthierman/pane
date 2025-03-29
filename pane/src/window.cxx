#include <pane/window.hxx>

namespace pane {
window::window(bool visible) {
    if (GetClassInfoExW(pane::system::module_handle().value_or(nullptr),
                        window_class.lpszClassName,
                        &window_class)
        == 0) {
        RegisterClassExW(&window_class);
    }

    ::CreateWindowExW(0,
                      window_class.lpszClassName,
                      window_class.lpszClassName,
                      WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | (visible ? WS_VISIBLE : 0),
                      CW_USEDEFAULT,
                      CW_USEDEFAULT,
                      CW_USEDEFAULT,
                      CW_USEDEFAULT,
                      nullptr,
                      nullptr,
                      window_class.hInstance,
                      this);
}

auto CALLBACK window::window_procedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
    -> LRESULT {
    if (msg == WM_NCCREATE) {
        auto create { reinterpret_cast<CREATESTRUCTW*>(lparam) };

        if (auto self { static_cast<Self*>(create->lpCreateParams) }; self) {
            SetWindowLongPtrW(hwnd, 0, reinterpret_cast<LONG_PTR>(self));
            self->window_handle = hwnd;
        }
    }

    return DefWindowProcW(hwnd, msg, wparam, lparam);
}
} // namespace pane
