#include "icu.hpp"

namespace pane {
auto icu_error::name() const noexcept -> const char* { return "icu"; }

auto icu_error::message(int value) const -> std::string {
    return u_errorName(static_cast<UErrorCode>(value));
}

const std::error_category& icu_error_category() {
    static icu_error instance;
    return instance;
}

auto make_error_code(UErrorCode error_code) -> std::error_code {
    return std::error_code(static_cast<int>(error_code), icu_error_category());
}
} // namespace pane
