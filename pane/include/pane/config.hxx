#pragma once
#include <glaze/glaze.hpp>
#include <expected>
#include <filesystem>
#include <system_error>
#include <pane/filesystem.hxx>
#include "../../src/glaze.hxx"

namespace pane {
struct glaze_test {
    std::string first;
    std::string second;
};

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

    auto to_json(this const Self& self) -> std::expected<std::u8string, std::error_code> {
        auto json { glz::write_json(self.settings) };

        if (!json.error()) {
            return std::unexpected(make_error_code(json.error().ec));
        }

        return std::u8string { json.value().begin(), json.value().end() };
    }

    // auto from_json(this const Self& self, std::u8string json)
    //     -> std::expected<std::u8string, std::error_code> {
    //     auto json { glz::read_json(self.settings, json) };

    //     return { json.begin(), json.end() };
    // }

    std::filesystem::path config_file {
        pane::filesystem::known_folder()
            .transform([](const std::filesystem::path& path) -> std::filesystem::path {
        return path / u"Phosphor" / u"config.json";
    }).value_or(u"")
    };

    T settings;
};
} // namespace pane
