#include "glaze.hxx"

namespace pane {
auto glz_error::name() const noexcept -> const char* { return "glz"; }

auto glz_error::message(int value) const -> std::string {
    return glz::format_error(static_cast<glz::error_code>(value));
}

const std::error_category& glz_error_category() {
    static glz_error instance;
    return instance;
}

auto make_error_code(glz::error_code error_code) -> std::error_code {
    return std::error_code(static_cast<int>(error_code), glz_error_category());
}
} // namespace pane
