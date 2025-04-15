#include <pane/error.hxx>

namespace pane {
error::error(HRESULT error)
    : error_code { error },
      message { pane::system::format_message(error) } { }

error::error(glz::error_ctx error)
    : error_code { static_cast<int64_t>(error.ec) } {
    auto formatted_error { glz::format_error(error.ec) };
    this->message = std::u8string { formatted_error.begin(), formatted_error.end() };
}
} // namespace pane
