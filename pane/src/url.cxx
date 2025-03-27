#include <pane/url.hxx>

namespace pane {
auto url::create(std::string_view string) -> std::optional<Self> {
    auto url { ada::parse<ada::url>(string.data()) };

    if (!url) {
        return std::nullopt;
    }

    Self self;
    self.storage = std::move(url.value());

    return self;
}

auto url::create(std::u8string_view string) -> std::optional<Self> {
    return create(reinterpret_cast<const char*>(string.data()));
}

auto url::create(const string& string) -> std::optional<Self> { return create(string.c_str()); }
} // namespace pane
