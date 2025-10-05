#pragma once
#include <string>
#include <glaze/glaze.hpp>

namespace pane {
template <typename T> auto to_json(const T& value) -> std::u8string {
    std::u8string buffer;

    [[maybe_unused]] auto ec { glz::write<glz::opts { .prettify { true } }>(value, buffer) };

    return buffer;
}

template <typename T> auto to_schema(const T& value) -> std::u8string {
    std::u8string buffer;

    [[maybe_unused]] auto ec { glz::write_json_schema<T>(buffer) };

    return buffer;
}
} // namespace pane
