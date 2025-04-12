#include <pane/error.hxx>

namespace pane {
error::error(HRESULT hresult)
    : error_code { hresult },
      message { pane::system::format_message(hresult) } { }
} // namespace pane
