#pragma once
#include <expected>
#include <filesystem>
#include <system_error>
#include <pane/filesystem.hpp>
#include <pane/glaze.hpp>

namespace pane {
template <typename T> struct config final {
    using Self = config;

    config() = default;
    ~config() = default;

    config(const Self&) = delete;
    auto operator=(const Self&) -> Self& = delete;

    config(Self&&) noexcept = delete;
    auto operator=(Self&&) noexcept -> Self& = delete;

    auto load(this Self& self) -> void {
        if (self.config_file.empty()) {
            return;
        }

        [[maybe_unused]] auto ec { glz::read_file_json(
            self.settings,
            reinterpret_cast<const char*>(self.config_file.u8string().data()),
            std::string {}) };
    }

    auto save(this const Self& self) -> void {
        if (self.config_file.empty()) {
            return;
        }

        [[maybe_unused]] auto ec { glz::write_file_json(
            self.settings,
            reinterpret_cast<const char*>(self.config_file.u8string().data()),
            std::string {}) };
    }

    auto to_json(this const Self& self) -> std::expected<std::u8string, std::error_code> {
        auto buffer { glz::write_json(self.settings) };

        if (!buffer) {
            return std::unexpected(make_error_code(buffer.error().ec));
        }

        return std::u8string { buffer.value().begin(), buffer.value().end() };
    }

    auto from_json(this Self& self, std::u8string json) -> std::expected<void, std::error_code> {
        auto buffer { glz::read_json<T>(json) };

        if (!buffer) {
            return std::unexpected(make_error_code(buffer.error().ec));
        }

        self.settings = buffer.value();

        return {};
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
