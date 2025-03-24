#include <pane/system.hxx>
#include <pane/string.hxx>
#include <pane/hstring.hxx>
#include <cstdlib>
#include <wil/resource.h>

namespace pane {
auto module_handle() -> ::HMODULE {
    ::HMODULE hmodule;

    ::GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT
                             | GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
                         reinterpret_cast<LPCWSTR>(&module_handle),
                         &hmodule);

    return hmodule;
}

auto format_message(::HRESULT errorCode) -> std::u8string {
    wil::unique_hlocal_string buffer;

    ::FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM
                         | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_MAX_WIDTH_MASK,
                     nullptr,
                     errorCode,
                     MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                     wil::out_param_ptr<wchar_t*>(buffer),
                     0,
                     nullptr);

    if (auto converted { pane::string::from_utf16(buffer.get()) }) {
        return converted.value().data;
    }

    return {};
}

auto last_error() -> std::u8string { return format_message(::GetLastError()); }

namespace brush {
    auto null() -> ::HBRUSH { return static_cast<::HBRUSH>(::GetStockObject(NULL_BRUSH)); }
} // namespace brush

namespace cursor {
    auto arrow() -> ::HCURSOR {
        return static_cast<::HCURSOR>(
            ::LoadImageW(nullptr, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED | LR_DEFAULTSIZE));
    }
} // namespace cursor

namespace icon {
    auto application() -> ::HICON {
        return static_cast<::HICON>(
            ::LoadImageW(nullptr, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_SHARED | LR_DEFAULTSIZE));
    }

    auto from_resource() -> ::HICON {
        return static_cast<::HICON>(
            ::LoadImageW(module_handle(), MAKEINTRESOURCEW(1), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE));
    }
} // namespace icon

auto message_loop() -> int {
    ::MSG msg {};
    int r {};

    while ((r = ::GetMessageW(&msg, nullptr, 0, 0)) != 0) {
        if (r == -1) {
            return EXIT_FAILURE;
        }

        else {
            ::TranslateMessage(&msg);
            ::DispatchMessageW(&msg);
        }
    }

    return static_cast<int>(msg.wParam);
}
} // namespace pane
