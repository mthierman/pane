#include <pane/pane.hxx>
#include <type_traits>
#include <print>
#include <system_error>

auto main() -> int {
    HRESULT hresult { S_OK };
    // auto error_code { std::error_code(hresult, std::generic_category()) };
    auto error_code { std::error_code(hresult, std::system_category()) };

    auto last_error { std::error_code(GetLastError(), std::system_category()) };

    std::println("last_error: {}, message: {}", last_error.value(), last_error.message());

    std::println("hresult: {}, message: {}", error_code.value(), error_code.message());

    std::println("hresult: {}, message: {}", hresult, pane::format_message(hresult));

    return 0;
}
