#include <cstdlib>
#include <format>
#include <print>

#include <pane/co_init.hxx>
#include <pane/file.hxx>

auto main() -> int {
    auto co_init { pane::co_init::multi_threaded() };

    auto temp_dir { pane::temp_folder() };

    if (temp_dir) {
        // auto file = pane::file(temp_dir.value());
        temp_dir.value() /= "test.txt";
        auto create { pane::create_file(temp_dir.value()) };

        std::println("{}, {}", temp_dir.value(), create);

        auto open { pane::open_file(temp_dir.value()) };

        if (open) {
            std::println("opened {}", temp_dir.value());
        }
    }

    return EXIT_SUCCESS;
}
