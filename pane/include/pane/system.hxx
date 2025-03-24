#pragma once
#include <Windows.h>
#include <optional>
#include <string>

namespace pane {
auto module_handle() -> ::HMODULE;
auto format_message(::HRESULT errorCode) -> std::u8string;
auto last_error() -> std::u8string;

namespace brush {
    auto null() -> ::HBRUSH;
}

namespace cursor {
    auto arrow() -> ::HCURSOR;
}

namespace icon {
    auto application() -> ::HICON;
    auto from_resource() -> ::HICON;
} // namespace icon

auto message_loop() -> int;
} // namespace pane
