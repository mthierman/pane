#pragma once
#include <string>
#include <glaze/glaze.hpp>

// template <> struct glz::meta<std::u8string> {
//     static constexpr auto value = [](auto&& self) { return std::string(self.begin(), self.end());
//     };
// };

namespace glz {
template <> struct to<JSON, std::u8string> {
    template <auto Opts>
    static void
    op(const std::u8string& value, is_context auto&& ctx, auto&& b, auto&& ix) noexcept {
        serialize<JSON>::op<Opts>(std::string { value.begin(), value.end() }, ctx, b, ix);
    }
};
} // namespace glz
