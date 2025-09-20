#include <pane/error.hpp>

namespace pane {
error::error(HRESULT error_code)
    : error_code { error_code },
      message { pane::system::format_message(error_code) } { }

error::error(glz::error_code error_code)
    : error_code { static_cast<int64_t>(error_code) } {
    auto formatted_error_code { glz::format_error(error_code) };
    this->message = { formatted_error_code.begin(), formatted_error_code.end() };
}

error::error(UErrorCode error_code)
    : error_code { static_cast<int64_t>(error_code) },
      message { reinterpret_cast<const char8_t*>(u_errorName(error_code)) } { }
} // namespace pane
