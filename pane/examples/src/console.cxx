#include <cstdlib>
#include <format>
#include <print>

#include <pane/guid.hxx>

auto main() -> int {
    auto guid { pane::guid::create() };

    if (guid) {
        std::println("{}", guid.value().string().value().get());
    }

    return EXIT_SUCCESS;
}
