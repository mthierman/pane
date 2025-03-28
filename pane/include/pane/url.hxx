#pragma once
#include <optional>
#include <string_view>
#include <ada.h>

namespace pane {
auto to_url(std::u8string_view string) -> std::optional<ada::url>;
} // namespace pane
