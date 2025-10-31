#include <pane/pane.hpp>
#include <cstdlib>
#include <format>
#include <print>
#include <string_view>

struct Settings {
    std::string one;
    std::string two;
};

auto wmain(int /* argc */, wchar_t* /* argv */[], wchar_t* /* envp */[]) -> int {
    auto args { pane::system::get_argv_u8() };

    auto task = [&](std::u8string_view cmp) -> bool { return args.at(1).compare(cmp) == 0; };

    if (task(u8"color")) {
        auto color { pane::color(0, 255, 0) };

        std::println("{}", color);
    }

    if (task(u8"config")) {
        std::println("config");

        pane::config<Settings> settings;

        auto json { settings.to_json() };

        if (json) {
            auto val { *json };
            std::println("{}", val);
        }
    }

    if (task(u8"error")) {
        std::println("error");

        HRESULT hr { 1 };

        auto error { pane::error(hr) };

        std::println("error_code: {} message: {}", error.error_code, error.message);
    }

    return EXIT_SUCCESS;
}
