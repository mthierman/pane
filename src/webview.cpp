#include <pane/webview.hpp>

namespace pane {
auto webview_token::operator()(this Self& self) -> EventRegistrationToken* { return &self.token; }
} // namespace pane
