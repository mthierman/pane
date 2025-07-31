#pragma once
#include <utility>
#include <type_traits>

template <typename E>
    requires std::is_enum_v<E>
constexpr auto operator+(E e) noexcept {
    return std::to_underlying(e);
}
