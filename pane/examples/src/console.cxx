#include <cstdlib>
#include <format>
#include <print>

#include <pane/guid.hxx>

auto main() -> int {
    auto guid { pane::guid::create() };

    if (guid) {
        std::println("{}", guid.value().to_string().value().get());
    }

    return EXIT_SUCCESS;
}
