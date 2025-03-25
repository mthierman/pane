#include <pane/pane.hxx>
#include <type_traits>
#include <print>
#include <system_error>

auto main() -> int {
    //
    // auto app_data { pane::path::from_known_folder() };

    // if (app_data) {
    //     std::println("{}", (*app_data).storage);
    // }

    // auto virt { pane::path::from_known_folder(FOLDERID_AddNewPrograms) };

    // if (!virt) {
    //     std::println("{}", virt.error().message());
    // }

    auto temp { pane::path::temp_folder() };

    if (temp) {
        std::println("{}", temp.value().storage);
    }

    return 0;
}
