#include <pane/color.hpp>
#include <pane/system.hpp>
#include <pane/text.hpp>
#include <shellapi.h>
#include <wil/resource.h>

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

    return pane::to_utf8_lossy(buffer.get());
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

// https://randomascii.wordpress.com/2018/12/03/a-not-called-function-can-cause-a-5x-slowdown/
auto get_argv(int argc, wchar_t** argv) -> std::vector<std::u16string> {
    std::vector<std::u16string> args;
    args.reserve(argc);

    for (auto warg : std::span(argv, argc)) {
        args.emplace_back(reinterpret_cast<char16_t*>(warg));
    }

    return args;
}

auto get_argv_u8(int argc, wchar_t** argv) -> std::vector<std::u8string> {
    std::vector<std::u8string> args;
    args.reserve(argc);

    for (auto warg : std::span(argv, argc)) {
        args.emplace_back(pane::to_utf8_lossy(warg));
    }

    return args;
}

auto get_argv() -> std::vector<std::u16string> {
    int argc { 0 };
    wil::unique_hlocal_ptr<wchar_t*[]> argv { CommandLineToArgvW(GetCommandLineW(), &argc) };

    std::vector<std::u16string> args;
    args.reserve(argc);

    for (auto warg : std::span(argv.get(), argc)) {
        std::wstring buffer { warg };
        args.emplace_back(buffer.begin(), buffer.end());
    }

    return args;
}

auto get_argv_u8() -> std::vector<std::u8string> {
    auto argv { get_argv() };

    std::vector<std::u8string> args;
    args.reserve(argv.size());

    for (auto& arg : argv) {
        args.emplace_back(pane::to_utf8_lossy(arg));
    }

    return args;
}

auto get_environment_variable(std::u8string_view name) -> std::optional<std::u8string> {
    auto u16name { pane::to_utf16_lossy(name) };
    auto buffer_size { GetEnvironmentVariableW(
        reinterpret_cast<const wchar_t*>(u16name.data()), nullptr, 0) };

    if (buffer_size == 0) {
        return std::nullopt;
    }

    std::u16string value;
    value.resize(buffer_size);
    auto value_size { GetEnvironmentVariableW(reinterpret_cast<const wchar_t*>(u16name.data()),
                                              reinterpret_cast<wchar_t*>(value.data()),
                                              buffer_size) };
    value.resize(value_size);

    return pane::to_utf8_lossy(value);
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
