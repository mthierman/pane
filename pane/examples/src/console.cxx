#include <cstdlib>
#include <format>
#include <print>

#include <pane/guid.hxx>

auto main() -> int {
    auto guid { pane::new_guid() };

    if (guid) {
        std::println("{}", guid.value());
    }

    return EXIT_SUCCESS;
}
