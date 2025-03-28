#include <pane/url.hxx>

namespace pane {
auto url::create(const string& string) -> std::optional<Self> {
    auto url { ada::parse<ada::url>(string.c_str()) };

    if (!url) {
        return std::nullopt;
    }

    Self self;
    self.storage = std::move(url.value());

    return self;
}
} // namespace pane
