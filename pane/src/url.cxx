#include <pane/url.hxx>

namespace pane {
auto url::parse(std::u8string_view string) -> std::optional<Self> {
    auto url { ada::parse<ada::url>(reinterpret_cast<const char*>(string.data())) };

    if (!url) {
        return std::nullopt;
    }

    Self self;
    self.storage = url.value();

    return self;
}

auto url::parse(const string& string) -> std::optional<Self> {
    auto url { ada::parse<ada::url>(string.c_str()) };

    if (!url) {
        return std::nullopt;
    }

    Self self;
    self.storage = url.value();

    return self;
}
} // namespace pane
