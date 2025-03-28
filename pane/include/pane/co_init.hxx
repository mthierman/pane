#pragma once
#include <Windows.h>
#include <objbase.h>

namespace pane {
struct co_init {
    using Self = co_init;

    ~co_init();

    co_init(Self&& co_init) noexcept = default;
    auto operator=(Self&& co_init) noexcept -> Self& = default;

    co_init(const Self& co_init) = delete;
    auto operator=(const Self& co_init) -> Self& = delete;

    operator HRESULT(this Self & self);

    static auto apartment_threaded() -> Self;
    static auto multi_threaded() -> Self;

    HRESULT result;

private:
    co_init() = default;
};
} // namespace pane
