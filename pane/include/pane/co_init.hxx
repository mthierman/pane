#pragma once
#include <Windows.h>
#include <objbase.h>

namespace pane {
struct co_init {
    using Self = co_init;

    ~co_init();

    co_init(Self&& co_init) noexcept = delete;
    auto operator=(Self&& co_init) noexcept -> Self& = delete;

    co_init(const Self& co_init) = delete;
    auto operator=(const Self& co_init) -> Self& = delete;

    operator HRESULT(this Self & self);

    static auto apartment_threaded() -> co_init;
    static auto multi_threaded() -> co_init;

    HRESULT result;

private:
    co_init(DWORD concurrency_model);
};
} // namespace pane
