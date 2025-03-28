#include <pane/url.hxx>

namespace pane {
url::url(ada::url&& url)
    : storage { std::move(url) } { }

auto url::create(const string& string) -> std::optional<Self> {
    auto url { ada::parse<ada::url>(string.c_str()) };

    if (!url) {
        return std::nullopt;
    }

    return Self(std::move(url.value()));
}
} // namespace pane
