#include <pane/system.hxx>
#include <shellapi.h>
#include <wil/resource.h>
#include <pane/color.hxx>

namespace pane::system {
auto hresult_error(HRESULT hresult) -> std::error_code {
    return std::error_code(HRESULT_CODE(hresult), std::system_category());
}

auto last_error() -> std::error_code {
    return std::error_code(GetLastError(), std::system_category());
}

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

auto format_message(HRESULT hresult) -> std::u8string {
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

    return pane::to_utf8(buffer.get());
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

auto resource_icon() -> std::expected<HICON, HRESULT> {
    auto module { module_handle() };

    if (!module) {
        return std::unexpected(module.error());
    }

    auto icon { static_cast<HICON>(
        LoadImageW(module.value(), MAKEINTRESOURCEW(1), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE)) };

    if (!icon) {
        auto last_error { GetLastError() };
        return std::unexpected(HRESULT_FROM_WIN32(last_error));
    }

    return icon;
}

auto ui_settings() -> winrt::Windows::UI::ViewManagement::UISettings {
    return winrt::Windows::UI::ViewManagement::UISettings();
}

auto dark_mode() -> bool {
    return pane::color { winrt::Windows::UI::ViewManagement::UIColorType::Background }.is_dark();
}

auto command_line_arguments() -> std::vector<std::u8string> {
    int argc { 0 };
    wil::unique_hlocal_ptr<wchar_t*[]> buffer;
    buffer.reset(CommandLineToArgvW(GetCommandLineW(), &argc));

    std::vector<std::u8string> vector;

    for (int i = 0; i < argc; i++) {
        vector.emplace_back(pane::to_utf8(buffer[i]));
    }

    return vector;
}

auto exit_process(unsigned int exit_code) -> void { ExitProcess(exit_code); }

auto quit(int exit_code) -> void { PostQuitMessage(exit_code); }

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
} // namespace pane::system
