#include <pane/pane.hxx>
#include <print>

auto main() -> int {
    auto temp_dir { pane::file::from_temp_folder() };

    if (temp_dir) {
        auto file = pane::file(temp_dir.value());
        file.storage /= "test.txt";
        auto create { file.create() };

        std::println("{}, {}", temp_dir.value().storage, create);

        auto open { file.open() };

        if (open) {
            std::println("{}", file.storage);
        }
    }

    return 0;
}
