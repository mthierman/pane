#include <pane/url.hxx>
#include <ada.h>

namespace pane {
auto url::from_string(string string) -> std::optional<Self> {
    auto url { ada::parse<ada::url>(string.c_str()) };

    if (!url) {
        return std::nullopt;
    }

    Self self;
    self.storage = url.value();

    return self;
}
} // namespace pane
