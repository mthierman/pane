#pragma once
#include <string>
#include <glaze/glaze.hpp>

namespace glz {
template <> struct from<JSON, std::u8string> {
    template <auto Opts>
    static void op(std::u8string& value, is_context auto&& ctx, auto&& it, auto&& end) {
        std::string buffer;
        parse<JSON>::op<Opts>(buffer, ctx, it, end);
        value.assign(buffer.begin(), buffer.end());
    }
};

template <> struct to<JSON, std::u8string> {
    template <auto Opts>
    static void
    op(const std::u8string& value, is_context auto&& ctx, auto&& b, auto&& ix) noexcept {
        serialize<JSON>::op<Opts>(std::string { value.begin(), value.end() }, ctx, b, ix);
    }
};
} // namespace glz
