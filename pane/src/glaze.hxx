#pragma once
#include <glaze/glaze.hpp>
#include <string>
#include <system_error>

namespace pane {
struct glz_error : std::error_category {
    auto name() const noexcept -> const char* override;
    auto message(int value) const -> std::string override;
};

auto make_error_code(glz::error_code error_code) -> std::error_code;
} // namespace pane

namespace std {
template <> struct is_error_code_enum<glz::error_code> : true_type { };
} // namespace std
