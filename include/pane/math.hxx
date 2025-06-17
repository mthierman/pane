#pragma once
#include <limits>
#include <random>

namespace pane {
template <typename T = int64_t, typename R = std::mt19937_64> auto random_number() -> T {
    constexpr T max { std::numeric_limits<T>::max() };
    thread_local R generator { std::random_device {}() };

    if constexpr (std::is_integral_v<T>) {
        thread_local std::uniform_int_distribution<T> dist(0, max);
        return dist(generator);
    }

    else if constexpr (std::is_floating_point_v<T>) {
        thread_local std::uniform_real_distribution<T> dist(0, max);
        return dist(generator);
    }
}
} // namespace pane
