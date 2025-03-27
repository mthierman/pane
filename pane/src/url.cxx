#include <pane/url.hxx>

namespace pane {
auto url::parse(std::string_view string) -> std::optional<Self> {
    auto url { ada::parse<ada::url>(string.data()) };

    if (!url) {
        return std::nullopt;
    }

    Self self;
    self.storage = url.value();

    return self;
}

auto url::parse(std::u8string_view string) -> std::optional<Self> {
    return parse(reinterpret_cast<const char*>(string.data()));
}

auto url::parse(const string& string) -> std::optional<Self> { return parse(string.c_str()); }
} // namespace pane
