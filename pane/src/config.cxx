#include <pane/config.hxx>

namespace pane {
struct test_glaze {
    std::string first;
    std::string second;
};

auto to_json() -> std::expected<std::u8string, std::error_code> {
    auto test { test_glaze() };
    auto buffer { glz::write_json(test) };

    if (!buffer) {
        return std::unexpected(make_error_code(buffer.error().ec));
    }

    return std::u8string { buffer.value().begin(), buffer.value().end() };
}

auto from_json(std::u8string json) -> std::expected<void, std::error_code> {
    auto buffer { glz::read_json<test_glaze>(json) };

    if (!buffer) {
        return std::unexpected(make_error_code(buffer.error().ec));
    }

    return {};
}
} // namespace pane
