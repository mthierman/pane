#include <pane/system.hxx>
#include <pane/string.hxx>
#include <pane/hstring.hxx>
#include <cstdlib>
#include <wil/resource.h>

namespace pane {
auto get_last_error() -> std::error_code {
    return std::error_code(GetLastError(), std::system_category());
}

auto module_handle() -> HMODULE {
    HMODULE hmodule;

    GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT
                           | GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
                       reinterpret_cast<LPCWSTR>(&module_handle),
                       &hmodule);

    return hmodule;
}

auto format_message(HRESULT error_code) -> std::u8string {
    wil::unique_hlocal_string buffer;

    FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM
                       | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_MAX_WIDTH_MASK,
                   nullptr,
                   error_code,
                   0,
                   wil::out_param_ptr<LPWSTR>(buffer),
                   0,
                   nullptr);

    if (auto converted { pane::string::from_utf16(buffer.get()) }) {
        return converted.value().data;
    }

    return {};
}

auto null_brush() -> HBRUSH { return static_cast<HBRUSH>(GetStockObject(NULL_BRUSH)); }

auto arrow_cursor() -> HCURSOR {
    return static_cast<HCURSOR>(
        LoadImageW(nullptr, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED | LR_DEFAULTSIZE));
}

auto application_icon() -> HICON {
    return static_cast<HICON>(
        LoadImageW(nullptr, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_SHARED | LR_DEFAULTSIZE));
}

auto resource_icon() -> HICON {
    return static_cast<HICON>(
        LoadImageW(module_handle(), MAKEINTRESOURCEW(1), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE));
}

auto message_loop() -> int {
    MSG msg {};
    int r {};

    while ((r = GetMessageW(&msg, nullptr, 0, 0)) != 0) {
        if (r == -1) {
            return EXIT_FAILURE;
        }

        else {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }

    return static_cast<int>(msg.wParam);
}
} // namespace pane
