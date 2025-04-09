#pragma once
#include <icu.h>
#include <string>
#include <system_error>

// http://blog.think-async.com/2010/04/system-error-support-in-c0x-part-4.html
// https://kb.firedaemon.com/support/solutions/articles/4000121648-fitting-com-into-c-system-error-handling

namespace pane {
struct icu_error : std::error_category {
    auto name() const noexcept -> const char* override;
    auto message(int value) const -> std::string override;
};

auto make_error_code(UErrorCode error_code) -> std::error_code;
} // namespace pane

namespace std {
template <> struct is_error_code_enum<UErrorCode> : true_type { };
} // namespace std
