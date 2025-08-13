#pragma once
#include <string>
#include <glaze/glaze.hpp>

template <> struct glz::meta<std::u8string> {
    static constexpr auto value = [](auto&& self) { return std::string(self.begin(), self.end()); };
};
