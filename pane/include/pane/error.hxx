#pragma once
#include <icu.h>
#include <string>
#include <system_error>

// http://blog.think-async.com/2010/04/system-error-support-in-c0x-part-4.html

struct icu_error : std::error_category {
    auto name() const noexcept -> const char* override;
    auto message(int ev) const -> std::string override;
};

auto make_error_code(UErrorCode) -> std::error_code;

namespace std {
template <> struct is_error_code_enum<UErrorCode> : true_type { };
} // namespace std
