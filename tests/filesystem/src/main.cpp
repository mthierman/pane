#include <pane/pane.hpp>
#include <print>
#include <filesystem>

auto wmain(int /* argc */, wchar_t* /* argv */[], [[maybe_unused]] wchar_t* envp[]) -> int {
    if (auto known_folder { pane::filesystem::known_folder(FOLDERID_LocalAppData) }) {
        auto value { known_folder.value() };

        std::println("{} - {}", value.u8string(), value.u8string().length());
    }

    if (auto temp_folder { pane::filesystem::temp_folder() }) {
        auto value { temp_folder.value() };

        std::println("{} - {}", value.u8string(), value.u8string().length());

        std::println("{} - {}",
                     std::filesystem::temp_directory_path().u8string(),
                     std::filesystem::temp_directory_path().u8string().length());
    }

    return 0;
}
