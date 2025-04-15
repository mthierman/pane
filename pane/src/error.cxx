#include <pane/error.hxx>

namespace pane {
error::error(HRESULT error)
    : error_code { error },
      message { pane::system::format_message(error) } { }

error::error(glz::error_code error)
    : error_code { static_cast<int64_t>(error) } {
    auto formatted_error { glz::format_error(error) };
    this->message = std::u8string { formatted_error.begin(), formatted_error.end() };
}

error::error(UErrorCode error)
    : error_code { static_cast<int64_t>(error) },
      message { reinterpret_cast<const char8_t*>(u_errorName(error)) } { }
} // namespace pane
