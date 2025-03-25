#include <pane/pane.hxx>
#include <type_traits>
#include <print>
#include <system_error>

auto main() -> int {
    HRESULT hresult { S_OK };

    std::println("{}", pane::format_message(hresult));

    return 0;
}
