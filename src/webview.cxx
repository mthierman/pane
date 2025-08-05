#include <pane/webview.hxx>

namespace pane {
auto webview_token::operator()(this Self& self) -> EventRegistrationToken* { return &self.token; }
} // namespace pane
