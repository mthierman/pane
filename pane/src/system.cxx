#include <pane/system.hxx>
#include <cstdlib>
#include <wil/resource.h>

namespace pane {
auto co_init_apartment() -> wil::unique_couninitialize_call {
    return wil::CoInitializeEx(COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
}

auto co_init_multi() -> wil::unique_couninitialize_call {
    return wil::CoInitializeEx(COINIT_MULTITHREADED | COINIT_DISABLE_OLE1DDE);
}

auto hresult_error(HRESULT hresult) -> std::error_code {
    return std::error_code(HRESULT_CODE(hresult), std::system_category());
}

auto last_error() -> std::error_code {
    return std::error_code(GetLastError(), std::system_category());
}

auto module_handle() -> std::expected<HMODULE, std::error_code> {
    HMODULE hmodule;

    if (GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT
                               | GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
                           reinterpret_cast<LPCWSTR>(&module_handle),
                           &hmodule)
        == 0) {
        return std::unexpected(last_error());
    };

    return hmodule;
}

auto format_message(HRESULT hresult) -> std::expected<string, std::error_code> {
    wil::unique_hlocal_string buffer;

    DWORD language_id;
    GetLocaleInfoEx(L"en-US",
                    LOCALE_RETURN_NUMBER | LOCALE_ILANGUAGE,
                    reinterpret_cast<LPWSTR>(&language_id),
                    sizeof(language_id) / sizeof(WCHAR));

    FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM
                       | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_MAX_WIDTH_MASK,
                   nullptr,
                   hresult,
                   language_id,
                   wil::out_param_ptr<LPWSTR>(buffer),
                   0,
                   nullptr);

    auto message { string::from_utf16(buffer.get()) };

    if (!message.has_value()) {
        return std::unexpected(last_error());
    }

    return message.value();
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

auto resource_icon() -> std::expected<HICON, std::error_code> {
    auto module { module_handle() };

    if (!module) {
        return std::unexpected(last_error());
    }

    auto icon { static_cast<HICON>(
        LoadImageW(module.value(), MAKEINTRESOURCEW(1), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE)) };

    if (!icon) {
        return std::unexpected(last_error());
    }

    return icon;
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
