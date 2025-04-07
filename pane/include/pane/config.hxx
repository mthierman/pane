#pragma once
#include <Windows.h>
#include <glaze/glaze.hpp>
#include <filesystem>

namespace pane {
template <typename T> struct config final {
    using Self = config;

    config() = default;
    ~config() = default;

    config(const Self& color) = delete;
    auto operator=(const Self& color) -> Self& = delete;
    config(Self&& color) noexcept = delete;
    auto operator=(Self&& color) noexcept -> Self& = delete;

    auto load(this Self& self) -> void {
        if (self.config_file.empty()) {
            return;
        }

        auto ec { glz::read_file_json(
            self.settings,
            reinterpret_cast<const char*>(self.config_file.u8string().data()),
            std::string {}) };
    }

    auto save(this const Self& self) -> void {
        if (self.config_file.empty()) {
            return;
        }

        auto ec { glz::write_file_json(
            self.settings,
            reinterpret_cast<const char*>(self.config_file.u8string().data()),
            std::string {}) };
    }

    std::filesystem::path config_file {
        pane::filesystem::known_folder()
            .transform([](const std::filesystem::path& path) -> std::filesystem::path {
        return path / u"Phosphor" / u"config.json";
    }).value_or(u"")
    };

    T settings;
};
} // namespace pane
