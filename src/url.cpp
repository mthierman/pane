#include <pane/url.hpp>

namespace pane {
auto url(std::u8string_view string) -> std::optional<ada::url> {
    auto url { ada::parse<ada::url>(reinterpret_cast<const char*>(string.data())) };

    if (!url) {
        return std::nullopt;
    }

    return url.value();
}
} // namespace pane
