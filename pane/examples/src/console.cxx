#include <pane/pane.hxx>
#include <type_traits>
#include <print>
#include <system_error>

auto main() -> int {
    // auto string { pane::string(u8"test") };
    // auto path { pane::path(string) };

    // std::println("{}", path.storage);

    HRESULT hresult { S_OK };

    auto code { HRESULT_CODE(hresult) };

    auto err { std::error_code(code, std::system_category()) };

    std::println("{}", err.message());

    return 0;
}
